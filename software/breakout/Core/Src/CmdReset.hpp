/*
 * CmdReset.cpp
 *
 *  Created on: Oct 5, 2020
 *      Author: adrian
 */

#ifndef SRC_CMDRESET_CPP_
#define SRC_CMDRESET_CPP_

#include "Shell.hpp"

class EPC901;
class Camera;

class CmdReset: public ShellCmd {
public:
	CmdReset(EPC901* sensor, Camera* camera);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
	Camera* _camera;
};

#endif /* SRC_CMDRESET_CPP_ */
