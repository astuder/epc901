/*
 * CmdTransfer.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
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
		 "\transfer all: Read all remaining images from frame buffer.\r\n");
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

	Frame* frame = _camera->readFrame();
	if (!frame) {
		shell->error("Frame buffer is empty!");
		return;
	}

	const char* param = shell->readParam();
	uint8_t read_all = 0;
	if (param && 0 == stricmp(param, "all")) {
		read_all = 1;
	}

	Leds::set(Leds::GREEN);

	do {
		char str[32];
		snprintf(str, 32, "%d,%d,%d", (int) frame->number, (int) frame->timestamp, (int) frame->exposure_time);
		shell->writeString(str);
		shell->newline();
		int i = 0;
		while (i < Frame::PIXEL_COUNT) {
			shell->writeHex(frame->pixels[i], 4);
			i++;
		}
		shell->newline();
	} while (read_all && (frame = _camera->readFrame()));

	Leds::clear(Leds::GREEN);
}


