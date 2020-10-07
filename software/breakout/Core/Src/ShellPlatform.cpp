/*
 * ShellPlatform.cpp
 *
 *  Created on: Oct 3, 2020
 *      Author: adrian
 */

#include "Shell.hpp"

// Platform specific implementation for Shell class

#include "main.h"
#include <cstring>

void Shell::_pfInit() {
}

unsigned char Shell::_pfReadChar() {
	// TODO: BUG: Breaks when keys are pressed while busy.
	char ch[1];
	if (HAL_OK == HAL_UART_Receive((UART_HandleTypeDef*) _pfdata, (uint8_t *)ch, 1, 0)) {
		return ch[0];
	} else {
		return 0;
	}
}

void Shell::_pfWriteChar(unsigned char ch) {
	char buf[1];
	buf[0] = ch;
	HAL_UART_Transmit((UART_HandleTypeDef*) _pfdata, (uint8_t*) buf, 1, 10);
}

void Shell::_pfWriteString(const char* str)
{
	if (str) {
		HAL_UART_Transmit((UART_HandleTypeDef*) _pfdata, (uint8_t*) str, strlen(str), 100);
	}
}
