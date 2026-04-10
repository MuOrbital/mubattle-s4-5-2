#pragma once

#include "ObjectManager.h"
#include "Viewport.h"
#include "User.h"
#include "MonsterAI.h"
#include "CriticalSection.h"
#include "LuaGameServer.h"
#include <thread>

class CTimer {
public:
	CTimer() {

	};

	~CTimer() {
		this->closed = true;

		if (this->luaTimer.joinable()) {
			this->luaTimer.detach();
		}
	};

	void start() {
		this->luaTimer = std::thread([this]() { this->runLuaTimer(); });

		this->closed = false;
	}

	void runLuaTimer() {
		while (!this->closed) {
			gLuaGameServer.TimerSystem();

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

private:
	std::thread luaTimer;

	bool closed = false;
};

extern CTimer gTimer;