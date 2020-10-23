/*
 * CmdReset.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: adrian
 */

#include "CmdReset.hpp"
#include "EPC901.hpp"
#include "Camera.hpp"
#include "Shell.hpp"
#include <cstring>

CmdReset::CmdReset(EPC901* sensor, Camera* camera) {
	init("reset",
		 "\treset mcu: Reset MCU.\r\n"
		 "\treset camera: Reset camera settings.\r\n"
		 "\treset sensor: Reset image sensor.\r\n");
	_sensor = sensor;
	_camera = camera;
}

void CmdReset::handler(Shell *shell) {
	if (!shell) {
		return;
	}

	if (!_sensor) {
		shell->error("No sensor!");
		return;
	}

	if (!_camera) {
		shell->error("No camera!");
	}

	const char* param = shell->readParam();
	if (!param) {
		shell->error("Missing device to reset.");
		return;
	}

	if (0 == stricmp(param, "sensor")) {
		_sensor->reset();
		shell->ok();
		return;
	}

	if (0 == stricmp(param, "camera")) {
		_camera->reset();
		shell->ok();
		return;
	}

	if (0 == stricmp(param, "mcu")) {
		shell->ok();
		NVIC_SystemReset();
		return;
	}

	shell->error("Unknow device!");
}
