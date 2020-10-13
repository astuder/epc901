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
		 "\tburst <on|fast|off>: Enable/disable burst mode.\r\n"
		 "\tburst frames <count>: Set number of frames to take in a burst.\r\n"
		 "\tburst interval <time_ms>: Set interval in milliseconds at which frames are taken.\r\n");
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
			if (_camera->getBurstFast()) {
				shell->writeString("FAST");
			} else {
				shell->writeString("ON");
			}
		} else {
			shell->writeString("OFF");
		}
		shell->newline();
		return;
	}

	if (0 == stricmp("on", param)) {
		_camera->setBurstEnable(1);
		_camera->setBurstFast(0);
		shell->ok();
		return;
	}

	if (0 == stricmp("fast", param)) {
		_camera->setBurstEnable(1);
		_camera->setBurstFast(1);
		shell->ok();
		return;
	}

	if (0 == stricmp("off", param)) {
		_camera->setBurstEnable(0);
		_camera->setBurstFast(0);
		shell->ok();
		return;
	}

	if (0 == stricmp("interval", param)) {
		param = shell->readParam();
		if (!param) {
			shell->writeInt(_camera->getBurstInterval());
			shell->newline();
			return;
		}
		uint16_t interval = shell->paramToUInt(param);
		_camera->setBurstInterval(interval);
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


