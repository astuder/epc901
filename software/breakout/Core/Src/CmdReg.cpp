/*
 * CmdReg.cpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#include "CmdReg.hpp"
#include "EPC901.hpp"
#include "Shell.hpp"

CmdRegRead::CmdRegRead(EPC901* sensor) {
	init("regread",
		 "\tregread <register>: Read value from I2C register of sensor.\r\n");
	_sensor = sensor;
}

void CmdRegRead::handler(Shell *shell) {
	if (!shell) {
		return;
	}

	if (!_sensor) {
		shell->error("No sensor!");
		return;
	}

	const char* param = shell->readParam();
	if (!param) {
		shell->error("Missing register address.");
		return;
	}

	uint8_t addr = shell->paramToUInt(param);
	uint8_t val = 0;
	uint8_t err = _sensor->readRegister(addr, &val);
	if (err == 0) {
		shell->writeString("0x");
		shell->writeHex(val, 2);
		shell->newline();
	} else {
		shell->error("I2C communication failed!");
	}
}

CmdRegWrite::CmdRegWrite(EPC901* sensor) {
	init("regwrite",
		 "\tregwrite <register> <value>: Write value to I2C register of sensor.\r\n");
	_sensor = sensor;
}

void CmdRegWrite::handler(Shell *shell) {
	if (!shell) {
		return;
	}

	if (!_sensor) {
		shell->error("No sensor!");
		return;
	}

	const char* param;
	param = shell->readParam();
	if (!param) {
		shell->error("Missing register address.");
		return;
	}
	const uint8_t addr = shell->paramToUInt(param);
	param = shell->readParam();
	if (!param) {
		shell->error("Missing value to write to register.");
		return;
	}
	uint8_t val = shell->paramToUInt(param);

	uint8_t err = _sensor->writeRegister(addr, val);
	if (err == 0) {
		shell->ok();
	} else {
		shell->error("I2C communication failed!");
	}
}
