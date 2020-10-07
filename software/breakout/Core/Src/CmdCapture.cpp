/*
 * CmdCapture.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#include "CmdCapture.hpp"
#include "Shell.hpp"
#include "Camera.hpp"
#include "Leds.hpp"
#include <cstring>

CmdCapture::CmdCapture(Camera* camera) {
	init("capture",
		 "\tcapture: Capture an image.\r\n");
	_camera = camera;
}

void CmdCapture::handler(Shell* shell) {
	if (!shell) {
		return;
	}

	if (!_camera) {
		shell->error("No camera!");
		return;
	}

	Leds::set(Leds::RED);
	uint8_t err = _camera->capture();
	Leds::clear(Leds::RED);
	switch (err) {
		case 0:
			shell->ok();
			break;
		case 1:
			shell->error("Sensor error!");
			break;
		case 2:
			shell->error("Frame buffer is full!");
			break;
		default:
			shell->error();
			break;
	}
}


