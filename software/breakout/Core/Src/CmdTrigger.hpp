/*
 * CmdTrigger.hpp
 *
 *  Created on: Oct 20, 2020
 *      Author: adrian
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
