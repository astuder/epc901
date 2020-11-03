/*
 * Shell commands to configure trigger.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDTRIGGER_HPP_
#define SRC_CMDTRIGGER_HPP_

#include "Shell.hpp"

class Camera;

class CmdTrigger: public ShellCmd {
public:
	CmdTrigger(Camera* camera);
	void handler(Shell* shell);
private:
	Camera* _camera;
};

#endif /* SRC_CMDTRIGGER_HPP_ */
