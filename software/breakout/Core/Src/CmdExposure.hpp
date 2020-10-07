/*
 * CmdExposure.hpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
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
