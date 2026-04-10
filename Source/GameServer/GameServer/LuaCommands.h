#ifndef LUACOMMANDS_H_
#define LUACOMMANDS_H_

#pragma once

#include "LuaState.h"
#include "CommandManager.h"

class CommandsClass {
public:
	CommandsClass() {

	};

	~CommandsClass() {

	};

	int getNumber(std::string string, int pos) const {
		return (int)gCommandManager.GetNumber((char*)string.c_str(), pos);
	};

	std::string getString(std::string string, int pos, int fullString) const {
		int count = 0, p = 0;

		std::string out;

		out.clear();

		if (fullString == 1) {
			for (size_t s = 0; s < string.size(); s++) {
				if (string[s] == 0x20) {
					out = &string[s];
					break;
				}
			}
		}
		else {
			for (size_t s = 0; s < string.size(); s++) {
				if (string[s] == 0x20) {
					count++;
				}
				else if (count == pos) {
					out.insert(p, 1, string[s]);
					p++;
				}
			}
		}

		return out.c_str();
	};
};

void RegisterCommandsClas(lua_State* lua);

#endif