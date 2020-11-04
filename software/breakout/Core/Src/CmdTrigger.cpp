/*
 * Shell commands to configure trigger.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include "CmdTrigger.hpp"

#include "Shell.hpp"
#include "Camera.hpp"
#include <cstring>
#include <cstdio>

CmdTrigger::CmdTrigger(Camera* camera) {
	init("trigger",
		 "\ttrigger <on|off>: Enable/disable trigger.\r\n"
		 "\ttrigger source <EXTERNAL | LEVEL | ZONE>: Select trigger source.\r\n"
		 "\ttrigger direction <RISING | FALLING>: For external trigger, set direction of edge.\r\n"
			"\t\t\tFor level and zone trigger, set whether signal crosses or not.\r\n"
		 "\ttrigger level <level>: For level trigger, set level above which to trigger.\r\n"
		 "\ttrigger zone <x1 y1 x2 y2>: For zone trigger, set rectangle within which to trigger.\r\n");
	_camera = camera;
}

void CmdTrigger::handler(Shell* shell) {
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

	const char* param = shell->readParam();
	if (!param) {
		if (_camera->getTriggerEnable()) {
			shell->writeString("ON");
		} else {
			shell->writeString("OFF");
		}
		shell->newline();
		return;
	}

	if (0 == stricmp("on", param)) {
		_camera->setTriggerEnable(1);
		shell->ok();
		return;
	}

	if (0 == stricmp("off", param)) {
		_camera->setTriggerEnable(0);
		shell->ok();
		return;
	}

	if (0 == stricmp("source", param)) {
		param = shell->readParam();
		if (!param) {
			switch (_camera->getTriggerSource()) {
			case Camera::TRIG_EXTERNAL:
				shell->writeString("EXTERNAL");
				break;
			case Camera::TRIG_LEVEL:
				shell->writeString("LEVEL");
				break;
			case Camera::TRIG_ZONE:
				shell->writeString("ZONE");
				break;
			default:
				shell->writeString("INVALID");
				break;
			}
			shell->newline();
			return;
		}

		if (0 == stricmp(param, "external")) {
			_camera->setTriggerSource(Camera::TRIG_EXTERNAL);
		} else if (0 == stricmp(param, "level")) {
			_camera->setTriggerSource(Camera::TRIG_LEVEL);
		} else if (0 == stricmp(param, "zone")) {
			_camera->setTriggerSource(Camera::TRIG_ZONE);
		} else {
			shell->error("invalid trigger source!");
			return;
		}

		shell->ok();
		return;
	}

	if (0 == stricmp("delay", param)) {
		param = shell->readParam();
		if (!param) {
			shell->writeInt(_camera->getTriggerDelay());
			shell->newline();
			return;
		}
		uint16_t delay = shell->paramToUInt(param);
		_camera->setTriggerDelay(delay);
		shell->ok();
		return;
	}

	if (0 == stricmp("direction", param)) {
		param = shell->readParam();
		if (!param) {
			if (Camera::TRIG_RISING == _camera->getTriggerDirection()) {
				shell->writeString("RISING");
			} else {
				shell->writeString("FALLING");
			}
			shell->newline();
			return;
		}
		if (0 == stricmp("rising", param)) {
			_camera->setTriggerDirection(Camera::TRIG_RISING);
			shell->ok();
			return;
		} else if (0 == stricmp("falling", param)) {
			_camera->setTriggerDirection(Camera::TRIG_FALLING);
			shell->ok();
			return;
		}
		shell->error("Invalid trigger direction!");
		return;
	}

	if (0 == stricmp("level", param)) {
		param = shell->readParam();
		if (!param) {
			shell->writeInt(_camera->getTriggerLevel());
			shell->newline();
			return;
		}
		uint16_t level = shell->paramToUInt(param);
		if (level > 4095) {
			level = 4095;
		}
		_camera->setTriggerLevel(level);
		shell->ok();
		return;
	}

	if (0 == stricmp("zone", param)) {
		Camera::trig_zone_t zone;
		param = shell->readParam();
		if (!param) {
			char zone_str[32];
			zone = _camera->getTriggerZone();
			snprintf(zone_str, 32, "%d %d %d %d", zone.x1, zone.y1, zone.x2, zone.y2);
			shell->writeString(zone_str);
			shell->newline();
			return;
		}

		if (param) {
			zone.x1 = shell->paramToUInt(param);
		}
		param = shell->readParam();
		if (param) {
			zone.y1 = shell->paramToUInt(param);
		}
		param = shell->readParam();
		if (param) {
			zone.x2 = shell->paramToUInt(param);
		}
		param = shell->readParam();
		if (param) {
			zone.y2 = shell->paramToUInt(param);
		}

		if (!param || zone.x1 > 1023 || zone.x2 > 1023 || zone.y1 > 4095 || zone.y2 > 4095) {
			shell->error("Invalid trigger zone!");
			return;
		} else {
			_camera->setTriggerZone(zone);
			shell->ok();
			return;
		}
	}
	shell->error("Invalid trigger command!");
}


