/*
 * Simple command shell.
 *
 * Copyright 2020 by Adrian Studer
 *
 * Licensed under the terms of MIT License, all text here must be included in any redistribution.
 * See the LICENSE-SW file for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <cstring>

#include "Shell.hpp"

static const char* _SH_Prompt = "> ";
static const char* _SH_Newline = "\r\n";

class ShellCmdHelp : public ShellCmd {
public:
	ShellCmdHelp();
	void handler(Shell* shell);
};

class ShellCmdEcho : public ShellCmd {
public:
	ShellCmdEcho();
	void handler(Shell* shell);
};

ShellCmdHelp::ShellCmdHelp(void) {
	init("help",
		 "\thelp: Display list of commands.\r\n"
		 "\thelp <command>: Display help text for command.\r\n");
}

void ShellCmdHelp::handler(Shell *shell) {
	shell->_helpCmdHandler();
}

ShellCmdEcho::ShellCmdEcho() {
	init("echo",
		 "\techo (on|off): Turn echo on or off.\r\n");
}

void ShellCmdEcho::handler(Shell *shell) {
	shell->_echoCmdHandler();
}

Shell::Shell() {
	static ShellCmdHelp cmd_help;
	static ShellCmdEcho cmd_echo;
	registerCmd(&cmd_help);
	registerCmd(&cmd_echo);
}

Shell::~Shell() {

}

void Shell::init(void* pfdata) {
	_input_buffer[INPUT_SIZE] = 0;	// 0-terminate buffer
	_input_len = 0;
	_input_pos = 0;
	_input_echo = 1;
	_input_mute = 0;
	_pfdata = pfdata;
	_pfInit();
	prompt();
	return;
}

void Shell::registerCmd(ShellCmd* cmd)
{
	if (CMD_COUNT_MAX > _cmd_count) {
		_commands[_cmd_count] = cmd;
		_cmd_count++;
	}
}

void Shell::loop(void)
{
	char ch = _pfReadChar();

	while (0 != ch) {
		switch (ch) {
		case '\n':
		case '\r':
			if (_input_echo && !_input_mute) {
				newline();
			}
			_processCommand();
			if (!_input_mute) {
				prompt();
			}
			_input_len = 0;
			_input_pos = 0;
			_input_mute = 0;
			return;
			break;

		case '\b':
		case 127:
			// Backspace or delete
			if (0 < _input_len) {
				_input_len--;
				if (_input_echo && !_input_mute) {
					writeChar(ch);
				}
			}
			break;

		case '@':
			// @ at start of line turns off echo for this command
			// Also removes verbose strings from OK and ERROR
			if (0 == _input_len) {
				_input_mute = 1;
				break;
			}
			// no break, intentionally left out to continue to default

		default:
			// printable characters
			if (0x20 <= ch && 0x7e >= ch) {
				if (INPUT_SIZE > _input_len) {
					_input_buffer[_input_len] = ch;
					_input_len++;
					if (_input_echo && !_input_mute) {
						// Echo accepted char
						writeChar(ch);
					}
				} else {
					// Line too long, ignore extra characters
				}
			} else {
				// ignored character
			}
			break;
		}
		ch = _pfReadChar();
	};

	return;
}

const char* Shell::readParam()
{
	// Skip whitespace
	while (_input_pos < _input_len
			&& isspace(_input_buffer[_input_pos])) {
		_input_pos++;
	}
	// Return 0 if no string available
	if (_input_pos >= _input_len) {
		return 0;
	}
	// Remember start position of string
	unsigned int str = _input_pos;
	// Find end of string (whitespace or comma)
	while (_input_pos < _input_len
			&& !isspace(_input_buffer[_input_pos])
			&& ',' != _input_buffer[_input_pos]) {
		_input_pos++;
	}
	// 0-terminate string
	_input_buffer[_input_pos] = 0;
	_input_pos++;

	return _input_buffer+str;
}

unsigned int Shell::paramToUInt(const char* str) const
{
	unsigned int x = 0;
	if (str) {
		if ('x' == str[1]) {
			// Read hex number
			str += 2;
			while (*str) {
				char ch = tolower(*str);
				if ('0' <= ch && '9' >= ch) {
					x = (x << 4) + (ch - '0');
				} else if ('a' <= ch && 'f' >= ch) {
					x = (x << 4) + (ch - 'a' + 10);
				} else {
					// Stop at any non-hex character
					break;
				}
				str++;
			}
		} else {
			x = strtoul(str, NULL, 10);
		}
	}
	return x;
}

void Shell::writeString(const char* str) {
	_pfWriteString(str);
}

void Shell::writeChar(unsigned char ch) {
	_pfWriteChar(ch);
}

void Shell::writeInt(int value)
{
	char str[16];
	writeString(itoa(value, str, 10));
}

void Shell::writeHex(unsigned int value, unsigned int digits)
{
	char str[16];
	snprintf(str, 16, "%08x", value);
	if (digits < 8) {
		writeString(&str[8-digits]);
	} else {
		writeString(str);
	}
}

void Shell::newline(void)
{
	writeString(_SH_Newline);
}

void Shell::prompt(void)
{
	writeString(_SH_Prompt);
}

void Shell::ok(const char* str)
{
	writeString("OK");
	if (!_input_mute && str) {
		writeChar(' ');
		writeString(str);
	}
	newline();
}

void Shell::error(const char* str)
{
	writeString("ERROR");
	if (!_input_mute && str) {
		writeChar(' ');
		writeString(str);
	}
	newline();
}

void Shell::_processCommand(void)
{
  unsigned int i;
  _input_pos = 0;

  const char* cmd = readParam();
  if (0 == strcmp(cmd, "?") || 0 == stricmp(cmd, "h")) {
      cmd = "help";
  }

  if (cmd && cmd[0] != 0) {
      i = 0;
      while (i < _cmd_count) {
    	  if (0 == stricmp(cmd, _commands[i]->_name)) {
    		  _commands[i]->handler(this);
    		  return;
    	  }
    	  i++;
      }

      // Command not found
      error("Unknown command");
  } else {
      // No command entered, do nothing
  }
}

void Shell::_helpCmdHandler(void)
{
	// Output help
	unsigned int i;
	const char* cmd = readParam();
	if (cmd) {
		i = 0;
		while (i < _cmd_count) {
			if (0 == strcmp(cmd, _commands[i]->_name)) {
				if (_commands[i]->_help) {
					writeString(_commands[i]->_help);
				} else {
					error("No help available for this command :(");
				}
				return;
			}
			i++;
		}
	}
	// Output list of available commands
	writeString("Shell commands:\r\n");
	i = 0;
	while (i < _cmd_count) {
		writeString("\t");
		writeString(_commands[i]->_name);
		newline();
		i++;
	}
	writeString("Type \"help <command>\" to display help text for a command.");
	newline();
	return;
}

void Shell::_echoCmdHandler(void)
{
	// Enable/disable echo
	const char* param = readParam();
	if (param) {
		if (0 == stricmp(param, "on")) {
			_input_echo = 1;
			ok("Echo is on.");
		} else if (0 == stricmp(param, "off")) {
			_input_echo = 0;
			ok("Echo is off.");
		} else {
			error("Invalid parameter. Use on or off to control echo.");
		}
	} else {
		if (0 == _input_echo) {
			writeString("OFF");
		} else {
			writeString("ON");
		}
		newline();
	}
}
