/*
 * Main application.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
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
