/*
 * Shell commands to access sensor registers.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDREG_HPP_
#define SRC_CMDREG_HPP_

#include "Shell.hpp"

class EPC901;

class CmdRegRead: public ShellCmd {
public:
	CmdRegRead(EPC901* sensor);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
};

class CmdRegWrite: public ShellCmd {
public:
	CmdRegWrite(EPC901* sensor);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
};

#endif /* SRC_CMDREG_HPP_ */
