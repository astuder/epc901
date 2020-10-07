/*
 * CmdReg.hpp
 *
 *  Created on: Oct 4, 2020
 *      Author: adrian
 */

#ifndef SRC_CMDREG_HPP_
#define SRC_CMDREG_HPP_

#include "Shell.hpp"

class EPC901;

class CmdRegRead: public ShellCmd {
public:
	CmdRegRead(EPC901* sensor);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
};

class CmdRegWrite: public ShellCmd {
public:
	CmdRegWrite(EPC901* sensor);
	void handler(Shell* shell);
private:
	EPC901* _sensor;
};

#endif /* SRC_CMDREG_HPP_ */
