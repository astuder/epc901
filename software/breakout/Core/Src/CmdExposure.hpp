/*
 * Shell command to configure exposure.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDEXPOSURE_HPP_
#define SRC_CMDEXPOSURE_HPP_

#include "Shell.hpp"

class Camera;

class CmdExposure: public ShellCmd {
public:
	CmdExposure(Camera* camera);
	void handler(Shell* shell);
private:
	Camera* _camera;
};

#endif /* SRC_CMDEXPOSURE_HPP_ */
