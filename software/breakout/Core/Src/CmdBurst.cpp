/*
 * CmdBurst.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: adrian
 */

#include "CmdBurst.hpp"

#include "Shell.hpp"
#include "Camera.hpp"
#include <cstring>

CmdBurst::CmdBurst(Camera* camera) {
	init("burst",
		 "\tburst <on|off>: Enable/disable burst mode.\r\n"
		 "\tburst frames <count>: Set number of frames to take in a burst.\r\n"
		 "\tburst delay <time_ms>: Set delay between frames in milliseconds.\r\n");
	_camera = camera;
}

void CmdBurst::handler(Shell* shell) {
	if (!shell) {
		return;
	}

	if (!_camera) {
		shell->error("No camera!");
		return;
	}

	const char* param = shell->readParam();
	if (!param) {
		if (_camera->getBurstEnable()) {
			shell->writeString("ON");
		} else {
			shell->writeString("OFF");
		}
		shell->newline();
		return;
	}

	if (0 == stricmp("on", param)) {
		_camera->setBurstEnable(1);
		shell->ok();
		return;
	}

	if (0 == stricmp("off", param)) {
		_camera->setBurstEnable(0);
		shell->ok();
		return;
	}

	if (0 == stricmp("delay", param)) {
		param = shell->readParam();
		if (!param) {
			shell->writeInt(_camera->getBurstDelay());
			shell->newline();
			return;
		}
		uint16_t delay = shell->paramToUInt(param);
		_camera->setBurstDelay(delay);
		shell->ok();
		return;
	}

	if (0 == stricmp("frames", param)) {
		param = shell->readParam();
		if (!param) {
			shell->writeInt(_camera->getBurstFrames());
			shell->newline();
			return;
		}
		uint16_t frames = shell->paramToUInt(param);
		if (frames < 1) {
			frames = 1;
		}
		_camera->setBurstFrames(frames);
		shell->ok();
		return;
	}

	shell->error("Invalid burst command!");
}


