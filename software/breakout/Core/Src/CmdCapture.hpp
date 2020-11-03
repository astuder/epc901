/*
 * Shell command to start image capture.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDCAPTURE_HPP_
#define SRC_CMDCAPTURE_HPP_

#include "Shell.hpp"

class Camera;

class CmdCapture: public ShellCmd {
public:
	CmdCapture(Camera* camera);
	void handler(Shell* shell);
private:
	Camera* _camera;
};

#endif /* SRC_CMDCAPTURE_HPP_ */
