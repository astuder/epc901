/*
 * Simple command shell.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#ifndef SRC_SHELL_HPP_
#define SRC_SHELL_HPP_

class Shell;

// Each shell command must inherit from this class
class ShellCmd {
public:
	void init(const char* name, const char* help) { _name = name; _help = help; };
	virtual void handler(Shell *shell) {};	// command handler function

protected:
	const char* _name;						// command name
	const char* _help;						// command help text

	friend Shell;
};

class ShellCmdHelp;
class ShellCmdEcho;

class Shell {
public:
	Shell();
	virtual ~Shell();

	void init(void* pfdata = 0);
	void registerCmd(ShellCmd* cmd);
	void loop(void);

	const char* readParam(void);
	unsigned int paramToUInt(const char* str) const;

	void writeChar(unsigned char ch);
	void writeString(const char* str);
	void writeInt(int value);
	void writeHex(unsigned int value, unsigned int digits);

	void newline(void);
	void prompt(void);
	void ok(const char* str = 0);
	void error(const char* str = 0);

private:
	void* _pfdata;

	void _processCommand(void);
	void _helpCmdHandler(void);
	void _echoCmdHandler(void);

	void _pfInit();
	unsigned char _pfReadChar();
	void _pfWriteChar(unsigned char ch);
	void _pfWriteString(const char* str);

	static const int INPUT_SIZE = 512;
	char _input_buffer[INPUT_SIZE+1] = { 0 }; 	// Extra byte provides space for 0-termination, init with empty string
	unsigned int _input_len = 0;
	unsigned int _input_pos = 0;
	int _input_echo = 1;
	int _input_mute = 0;

	static const int CMD_COUNT_MAX = 16;
	ShellCmd* _commands[CMD_COUNT_MAX];
	unsigned int _cmd_count = 0;

	friend ShellCmdHelp;
	friend ShellCmdEcho;
};

#endif /* SRC_SHELL_HPP_ */
