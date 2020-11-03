/*
 * Simple command shell - platform specific code.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include "Shell.hpp"

// Platform specific implementation for Shell class

#include "main.h"
#include <cstring>

// UART RX ring buffer, filled by DMA. Will lose data on overflow, but that's ok.
#define RX_BUF_SIZE 64
static uint8_t rx_buffer[RX_BUF_SIZE];
static uint32_t rx_pos;
#define DMA_WRITE_POS ( (RX_BUF_SIZE - ((UART_HandleTypeDef*) _pfdata)->hdmarx->Instance->CNDTR) & (RX_BUF_SIZE - 1) )

void Shell::_pfInit() {
	HAL_UART_Receive_DMA((UART_HandleTypeDef*) _pfdata, rx_buffer, RX_BUF_SIZE);
	rx_pos = 0;
}

unsigned char Shell::_pfReadChar() {
	unsigned char ch = 0;
	if (rx_pos != DMA_WRITE_POS) {
		ch = rx_buffer[rx_pos];
		rx_pos++;
		rx_pos &= (RX_BUF_SIZE - 1);
	}
	return ch;
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
