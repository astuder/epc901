/*
 * CmdCapture.hpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
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
