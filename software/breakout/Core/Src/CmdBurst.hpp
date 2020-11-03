/*
 * Shell commands to configure burst.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDBURST_HPP_
#define SRC_CMDBURST_HPP_

#include "Shell.hpp"

class Camera;

class CmdBurst: public ShellCmd {
public:
	CmdBurst(Camera* camera);
	void handler(Shell* shell);
private:
	Camera* _camera;
};

#endif /* SRC_CMDBURST_HPP_ */
