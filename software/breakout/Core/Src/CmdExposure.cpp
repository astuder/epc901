/*
 * CmdExposure.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#include "CmdExposure.hpp"
#include "Shell.hpp"
#include "Camera.hpp"
#include <cstring>

CmdExposure::CmdExposure(Camera* camera) {
	init("exposure",
		 "\texposure: Get current exposure time in microseconds.\r\n"
		 "\texposure <time>: Set exposure time in microseconds.\r\n");
	_camera = camera;
}

void CmdExposure::handler(Shell* shell) {
	if (!shell) {
		return;
	}

	if (!_camera) {
		shell->error("No camera!");
		return;
	}

	const char* param = shell->readParam();
	if (!param) {
		shell->writeInt(_camera->getExposureTime());
		shell->newline();
		return;
	}

	uint32_t time_us = shell->paramToUInt(param);
	if (0 == time_us || 0 == stricmp("auto", param)) {
		// TODO: Auto exposure
		shell->error("Auto exposure not implemented yet.");
		return;
	}

	_camera->setExposureTime(time_us);
	shell->ok();
}



