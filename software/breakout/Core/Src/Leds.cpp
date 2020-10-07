/*
 * LEDManager.cpp
 *
 *  Created on: Sep 26, 2020
 *      Author: adrian
 */

#include "Leds.hpp"

#include "main.h"
#include <cstring>

const uint8_t LED_COUNT = 3;
GPIO_TypeDef* LED_PORTS[] = { LD1_GPIO_Port, LD2_GPIO_Port, LD3_GPIO_Port };
const uint16_t LED_PINS[] = { LD1_Pin, LD2_Pin, LD3_Pin };

Leds::Leds() {
	memset(mLedState, 0, sizeof(mLedState));
}

Leds::~Leds() {
}

Leds* Leds::getInstance(void) {
	static Leds instance;
	return &instance;
}

void Leds::clearAll(void) {
	for (uint8_t i=0; i<LED_COUNT; i++) {
		clear(i);
	}
}

void Leds::clear(uint8_t led) {
	if (led >= LED_COUNT) {
		// invalid led number
		return;
	}

	HAL_GPIO_WritePin(LED_PORTS[led], LED_PINS[led], GPIO_PIN_RESET);
	Leds::getInstance()->mLedState[led] = 0;
}

void Leds::set(uint8_t led) {
	if (led >= LED_COUNT) {
		// invalid led number
		return;
	}

	HAL_GPIO_WritePin(LED_PORTS[led], LED_PINS[led], GPIO_PIN_SET);
	Leds::getInstance()->mLedState[led] = 1;
}
