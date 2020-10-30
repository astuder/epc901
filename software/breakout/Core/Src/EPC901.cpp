/*
 * EPC901.cpp
 *
 *  Created on: Sep 26, 2020
 *      Author: adrian
 */

#include "main.h"
#include "EPC901.hpp"
#include "dwt_stm32_delay.h"
#include <inttypes.h>
#include <cstring>

const uint8_t EPC_I2C_ADDR = 0x15 << 1;

EPC901::EPC901() {
	_chip_rev = 0;
	_i2c_addr = 0;
	_i2c_handle = 0;
	_power_state = 0;
}

EPC901::~EPC901() {
}

uint8_t EPC901::init(I2C_HandleTypeDef* i2c_handle, uint8_t i2c_addr, SPI_HandleTypeDef* spi_handle, TIM_HandleTypeDef* tim_handle) {
	_i2c_handle = i2c_handle;
	_i2c_addr = i2c_addr << 1;
	_spi_handle = spi_handle;
	_tim_handle = tim_handle;

	// set all pins to sane defaults
	HAL_GPIO_WritePin(PWR_DOWN_GPIO_Port, PWR_DOWN_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CLR_DATA_GPIO_Port, CLR_DATA_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CLR_PIX_GPIO_Port, CLR_PIX_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(READ_GPIO_Port, READ_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(ADC_CS_GPIO_Port, ADC_CS_Pin, GPIO_PIN_SET);

	// configure and enable SPI peripheral
	__HAL_LOCK(_spi_handle);
	SET_BIT(_spi_handle->Instance->CR2, SPI_RXFIFO_THRESHOLD);
	__HAL_SPI_ENABLE(_spi_handle);
	__HAL_UNLOCK(_spi_handle);

	// enable one pulse timer
	HAL_TIM_OnePulse_Start(_tim_handle, TIM_CHANNEL_1);
	// enable CH3 which is not full supported in HAL
	TIM_CCxChannelCmd(_tim_handle->Instance, TIM_CHANNEL_3, TIM_CCx_ENABLE);
	// dummy pulse to get timer into known state
	__HAL_TIM_SET_PRESCALER(_tim_handle, 0);
	_tim_handle->Instance->CCR3 = 1;
	_tim_handle->Instance->ARR = 10;
	__HAL_TIM_ENABLE(_tim_handle);
	HAL_Delay(1);

	// init sensor
	_powerUp();

	// read chip revision

	uint8_t revision;
	if (HAL_OK == HAL_I2C_Mem_Read(_i2c_handle, EPC_I2C_ADDR, 0xff, 1, &revision, 1, 100)) {
		_chip_rev = revision;
	} else {
		_powerDown();
		return 1;
	}

	// TODO: oscillator calibration
	// may be negligible for exposure times >1us

	_powerDown();

	return 0;
}

uint16_t EPC901::captureImage(uint32_t exposure_us, uint16_t* buffer) {
	if (exposure_us > _max_exposure_us) {
		return 0;
	}
	_powerUp();
	_clear();
	_exposeImage(exposure_us);
	uint16_t pixels = _readImage(buffer);
	_powerDown();
	return pixels;
}

uint8_t EPC901::getRevision() {
	return _chip_rev;
}

uint8_t EPC901::readRegister(uint8_t address, uint8_t* value) {
	uint8_t val = 0;
	uint8_t err = 0;
	_powerUp();
	err = HAL_I2C_Mem_Read(_i2c_handle, EPC_I2C_ADDR, address, 1, &val, 1, 100);
	if (err == HAL_OK) {
		*value = val;
	}
	_powerDown();
	return err;
}

uint8_t EPC901::writeRegister(uint8_t address, uint8_t value) {
	uint8_t val = value;
	uint8_t err = 0;
	_powerUp();
	err = HAL_I2C_Mem_Write(_i2c_handle, EPC_I2C_ADDR, address, 1, &val, 1, 100);
	_powerDown();
	return err;
}

uint8_t EPC901::reset(void) {
	uint8_t val = 6;
	uint8_t err = 0;
	_powerUp();
	err = HAL_I2C_Master_Transmit(_i2c_handle, 0, &val, 1, 100);
	_powerDown();
	return err;
}

void EPC901::_powerUp() {
	// power up sensor
	HAL_GPIO_WritePin(PWR_DOWN_GPIO_Port, PWR_DOWN_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(12);
	_power_state = 1;
}

void EPC901::_powerDown() {
	// power down sensor
	HAL_GPIO_WritePin(PWR_DOWN_GPIO_Port, PWR_DOWN_Pin, GPIO_PIN_SET);
	_power_state = 0;
}

void EPC901::_clear() {
	// clear data (necessary when using power-down?)
	HAL_GPIO_WritePin(CLR_DATA_GPIO_Port, CLR_DATA_Pin, GPIO_PIN_SET);
	DWT_Delay_us(2);
	HAL_GPIO_WritePin(CLR_DATA_GPIO_Port, CLR_DATA_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(20);

	// clear pixels (necessary when using power-down?)
	HAL_GPIO_WritePin(CLR_PIX_GPIO_Port, CLR_PIX_Pin, GPIO_PIN_SET);
	DWT_Delay_us(2);
	HAL_GPIO_WritePin(CLR_PIX_GPIO_Port, CLR_PIX_Pin, GPIO_PIN_RESET);
	DWT_Delay_us(20);
}

void EPC901::_exposeImage(uint32_t exposure_us) {
	if (_power_state == 0) {
		_powerUp();
		_clear();
	}

	// calculate exposure in clk ticks
	uint32_t exposure_clk = exposure_us * 80;
	// set prescaler
	if (exposure_clk >= 0xffffff) {
		exposure_clk = exposure_clk >> 16;
		__HAL_TIM_SET_PRESCALER(_tim_handle, 0xffff);
	} else if (exposure_clk >= 0xffff) {
		exposure_clk = exposure_clk >> 8;
		__HAL_TIM_SET_PRESCALER(_tim_handle, 0xff);
	} else {
		__HAL_TIM_SET_PRESCALER(_tim_handle, 0);
	}
	// set pulse_start
	_tim_handle->Instance->CCR3 = 1;
	// set pulse end
	_tim_handle->Instance->ARR = 1 + exposure_clk;
	// start pulse
	__HAL_TIM_ENABLE(_tim_handle);

	// timer stops automatically when done
}

// Reading pixel from sensor using ADC on breakout board:
//   set ADC CS low - sample HOLD
//   start SPI transfer of 2 bytes from ADC
//   set READ high
//   wait for and read 1 byte from SPI/ADC
//   set READ low
//   wait for and read 1 byte from SPI/ADC (ADC automatically switches to SAMPLE on 13th bit)
//   set ADC CS high
#define __readADC() ({	\
		ADC_CS_GPIO_Port->BRR = (uint32_t)ADC_CS_Pin; \
		spi->DR = (uint16_t) 0; \
		READ_GPIO_Port->BSRR = (uint32_t)READ_Pin; \
		while (!(spi->SR & SPI_FLAG_RXNE)); \
		READ_GPIO_Port->BRR = (uint32_t)READ_Pin; \
		data_msb = *(__IO uint8_t *)&spi->DR; \
		while (!(spi->SR & SPI_FLAG_RXNE)); \
		data_lsb = *(__IO uint8_t *)&spi->DR; \
		ADC_CS_GPIO_Port->BSRR = (uint32_t)ADC_CS_Pin; \
		})
// don't clock READ during last ADC conversion
#define __finishADC() ({	\
		ADC_CS_GPIO_Port->BRR = (uint32_t)ADC_CS_Pin; \
		spi->DR = (uint16_t) 0; \
		while (!(spi->SR & SPI_FLAG_RXNE)); \
		data_msb = *(__IO uint8_t *)&spi->DR; \
		while (!(spi->SR & SPI_FLAG_RXNE)); \
		data_lsb = *(__IO uint8_t *)&spi->DR; \
		ADC_CS_GPIO_Port->BSRR = (uint32_t)ADC_CS_Pin; \
		})


uint16_t EPC901::_readImage(uint16_t* buffer) {
	// wait for data ready
	while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(DATA_RDY_GPIO_Port, DATA_RDY_Pin)) {
		// TODO: timeout?
	}

	// read pulse
	HAL_GPIO_WritePin(READ_GPIO_Port, READ_Pin, GPIO_PIN_SET);
	// wait 3 osc cycles - ~0.1us with osc ~36MHz
	DWT_Delay_us(1);
	HAL_GPIO_WritePin(READ_GPIO_Port, READ_Pin, GPIO_PIN_RESET);
	// wait for conversion (37-3 osc cycles) - ~1us with osc ~36MHz
	DWT_Delay_us(2);	// TODO: can be reduced to 1us with calibrated oscillator

	// lock SPI handle
	__HAL_LOCK(_spi_handle);
	SPI_TypeDef* spi = _spi_handle->Instance;

	// read pixels
	static const uint16_t pixel_cnt = 1024;		// TODO: Replace to support binning
	uint16_t count;
	uint8_t data_msb, data_lsb;

	// discard first 4 samples (3 pre-loading cycles, 1 ADC sample/conversion pipeline)
	count = 4;
	do {
		__readADC();
		count--;
	} while (count != 0);

	// read pixels
	count = pixel_cnt - 1;
	do {
		__readADC();
		// store pixel
		*buffer = (data_msb << 7) | (data_lsb >> 1);
		buffer++;
		count--;
	} while (count != 0);

	// Don't wiggle READ when reading last pixel
	__finishADC();
	*buffer = (data_msb << 7) | (data_lsb >> 1);

	// release SPI peripheral
	__HAL_UNLOCK(_spi_handle);

	return pixel_cnt;
}

uint8_t EPC901::_dataReady() {
	return GPIO_PIN_SET == HAL_GPIO_ReadPin(DATA_RDY_GPIO_Port, DATA_RDY_Pin);
}
