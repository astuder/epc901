/*
 * Shell commands to transfer image data.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include "CmdTransfer.hpp"
#include "Shell.hpp"
#include "Camera.hpp"
#include "Leds.hpp"
#include <stdio.h>
#include <cstring>

CmdTransfer::CmdTransfer(Camera* camera) {
	init("transfer",
		 "\transfer: Read next image from frame buffer.\r\n"
		 "\transfer all: Read all remaining images from frame buffer.\r\n"
		 "\transfer last: Read most recent image from frame buffer, discard the rest.\r\n");
	_camera = camera;
}

void CmdTransfer::handler(Shell* shell) {
	if (!shell) {
		return;
	}

	if (!_camera) {
		shell->error("No camera!");
		return;
	}

	if (_camera->getState() != Camera::ST_IDLE) {
		shell->writeString("BUSY");
		shell->newline();
		return;
	}

	Frame* frame = _camera->readFrame();
	if (!frame) {
		shell->error("Frame buffer is empty!");
		return;
	}

	const char* param = shell->readParam();
	uint8_t read_all = 0;
	uint8_t read_last = 0;
	if (param) {
		if (0 == stricmp(param, "all")) {
			read_all = 1;
		} else if (0 == stricmp(param, "last")) {
			read_last = 1;
		}
	}

	Leds::set(Leds::GREEN);

	// discard all frames except last
	while (read_last && _camera->getRemainingFrames() != 0) {
			frame = _camera->readFrame();
	}

	do {
		char str[32];
		snprintf(str, 32, "%d,%d,%d,%d", (int) frame->number, (int) frame->timestamp, (int) frame->exposure_time, (int) frame->pixel_cnt);
		shell->writeString(str);
		shell->newline();
		int i = 0;
		while (i < frame->pixel_cnt) {
			shell->writeHex(frame->pixels[i], 3);
			i++;
		}
		shell->newline();
	} while (read_all && (frame = _camera->readFrame()));

	Leds::clear(Leds::GREEN);
}


