/*
 * App.cpp
 *
 *  Created on: Sep 26, 2020
 *      Author: adrian
 */

#include "main.h"
#include "App.hpp"
#include "Shell.hpp"
#include "EPC901.hpp"
#include "Camera.hpp"
#include "CmdReg.hpp"
#include "CmdReset.hpp"
#include <stdlib.h>
#include <cstring>
#include "Leds.hpp"

const char chiprev_str[] = "ECP901 rev ";
const char new_line[] = "\n\r";
const char comma[] = ", ";

#define BUFFERED_FRAMES 50

Frame frame_buffer[BUFFERED_FRAMES];

EPC901 sensor = EPC901();
Camera camera = Camera();

Shell shell = Shell();
CmdRegRead cmd_regread = CmdRegRead(&sensor);
CmdRegWrite cmd_regwrite = CmdRegWrite(&sensor);
CmdReset cmd_reset = CmdReset(&sensor);

void App_Setup(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart, SPI_HandleTypeDef* hspi) {
	Leds::clearAll();
	sensor.init(hi2c, 0x15, hspi);
	shell.init(huart);
	shell.registerCmd(&cmd_regread);
	shell.registerCmd(&cmd_regwrite);
	shell.registerCmd(&cmd_reset);
	camera.init(&sensor, &shell, frame_buffer, BUFFERED_FRAMES);
	return;
}

void App_Loop() {
	shell.loop();
/*
	leds.set(leds.BLUE);
	char rev_str[10];
	HAL_UART_Transmit(h_app_uart, (uint8_t*) chiprev_str, strlen(chiprev_str), -1);
	itoa(sensor.getRevision(), rev_str, 10);
	HAL_UART_Transmit(h_app_uart, (uint8_t*) rev_str, strlen(rev_str), -1);
	HAL_UART_Transmit(h_app_uart, (uint8_t*) new_line, strlen(new_line), -1);

	uint16_t *image = 0;
	image = sensor.captureImage(25);
	if (image) {
		char pixel_str[10];
		for (int i=0; i<1024; i++) {
			itoa(image[i], pixel_str, 10);
			HAL_UART_Transmit(h_app_uart, (uint8_t*) pixel_str, strlen(pixel_str), -1);
			if (i != 1023) {
				HAL_UART_Transmit(h_app_uart, (uint8_t*) comma, strlen(comma), -1);
			}
			if ((i & 0x07) == 0x07) {
				HAL_UART_Transmit(h_app_uart, (uint8_t*) new_line, strlen(new_line), -1);
			}
		}
		HAL_UART_Transmit(h_app_uart, (uint8_t*) new_line, strlen(new_line), -1);
	}
	HAL_Delay(1000);
	leds.clear(leds.BLUE);
	HAL_Delay(1000);
*/
	return;
}
