/*
 * CmdBurst.hpp
 *
 *  Created on: Oct 5, 2020
 *      Author: adrian
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
