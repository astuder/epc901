/*
 * CmdTransfer.hpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
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
