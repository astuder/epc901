/*
 * LEDManager.h
 *
 *  Created on: Sep 26, 2020
 *      Author: adrian
 */

#ifndef SRC_LEDS_HPP_
#define SRC_LEDS_HPP_

#include <inttypes.h>

class Leds {
public:
	static const uint8_t GREEN = 0;
	static const uint8_t BLUE = 1;
	static const uint8_t RED = 2;

	virtual ~Leds();
	static Leds* getInstance(void);
	static void clearAll(void);
	static void clear(uint8_t led);
	static void set(uint8_t led);

private:
	Leds();

	uint8_t mLedState[3];
};

#endif /* SRC_LEDS_HPP_ */
