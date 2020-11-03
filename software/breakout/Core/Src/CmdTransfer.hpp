/*
 * Shell commands to transfer image data.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_CMDTRANSFER_HPP_
#define SRC_CMDTRANSFER_HPP_

#include "Shell.hpp"

class Camera;

class CmdTransfer: public ShellCmd {
public:
	CmdTransfer(Camera* camera);
	void handler(Shell* shell);
private:
	Camera* _camera;
};

#endif /* SRC_CMDTRANSFER_HPP_ */
