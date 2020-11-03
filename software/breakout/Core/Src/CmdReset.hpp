/*
 * Shell commands to reset components.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDRESET_CPP_
#define SRC_CMDRESET_CPP_

#include "Shell.hpp"

class EPC901;
class Camera;

class CmdReset: public ShellCmd {
public:
	CmdReset(EPC901* sensor, Camera* camera);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
	Camera* _camera;
};

#endif /* SRC_CMDRESET_CPP_ */
