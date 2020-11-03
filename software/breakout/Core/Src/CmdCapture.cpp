/*
 * Shell command to start image capture.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include "CmdCapture.hpp"
#include "Shell.hpp"
#include "Camera.hpp"
#include "Leds.hpp"
#include <cstring>

CmdCapture::CmdCapture(Camera* camera) {
	init("capture",
		 "\tcapture: Capture an image.\r\n"
		 "\tcapture abort: Abort current capture.\r\n");
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

	const char* param = shell->readParam();

	if (param) {
		if (0 == stricmp(param, "ABORT")) {
			_camera->abort();
		}
		shell->ok();
		return;
	}

	if (_camera->getState() != Camera::ST_IDLE) {
		shell->writeString("BUSY");
		shell->newline();
		return;
	}

	uint8_t err = _camera->capture();

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


