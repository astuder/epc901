/*
 * App.h
 *
 *  Created on: Sep 26, 2020
 *      Author: adrian
 */

#ifndef SRC_APP_HPP_
#define SRC_APP_HPP_

#ifdef __cplusplus
	extern "C" {
#endif

void App_Setup(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart, SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim);
void App_Loop();
void App_Interrupt();

#ifdef __cplusplus
	}
#endif

#endif /* SRC_APP_HPP_ */
