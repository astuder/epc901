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

#define BUFFERED_FRAMES 100

Frame frame_buffer[BUFFERED_FRAMES];

EPC901 sensor = EPC901();
Camera camera = Camera();

Shell shell = Shell();
CmdRegRead cmd_regread = CmdRegRead(&sensor);
CmdRegWrite cmd_regwrite = CmdRegWrite(&sensor);
CmdReset cmd_reset = CmdReset(&sensor, &camera);

void App_Setup(I2C_HandleTypeDef* hi2c, UART_HandleTypeDef* huart, SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim) {
	Leds::clearAll();
	sensor.init(hi2c, 0x15, hspi, htim);
	shell.init(huart);
	shell.registerCmd(&cmd_regread);
	shell.registerCmd(&cmd_regwrite);
	shell.registerCmd(&cmd_reset);
	camera.init(&sensor, &shell, frame_buffer, BUFFERED_FRAMES);
	return;
}

void App_Loop() {
	shell.loop();
	camera.loop();
	if (camera.getState() == Camera::ST_IDLE) {
		Leds::clear(Leds::RED);
	} else {
		Leds::set(Leds::RED);
	}
	return;
}

void App_Interrupt() {
	camera.externalTrigger();
}
