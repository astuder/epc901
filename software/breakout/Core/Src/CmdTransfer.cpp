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
		 "\transfer: Read image from frame buffer.\r\n");
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

	Leds::set(Leds::GREEN);
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
	Leds::clear(Leds::GREEN);
}


