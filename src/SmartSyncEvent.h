/*
* This file is part of SmartSyncEvent Library.
* Copyright (C) 2023 Alejandro Nicolini
* 
* SmartSyncEvent is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* SmartSyncEvent is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with SmartSyncEvent. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ___SMART_SYNC_EVENT_H___
#define ___SMART_SYNC_EVENT_H___

#include <Arduino.h>
#include <string>
#include <map>

class SmartSyncEvent {
public:
	SmartSyncEvent();
	~SmartSyncEvent();

	struct Result {
		bool triggered;
		unsigned int event_id;

		// User-defined conversion to bool
		operator bool() const {
			return triggered;
		}
	};

	static Result trigger(int ms, const std::string& file, int line);
	static void reset(unsigned int event_id);
	
private:
	static const int MAX_INSTANCES = 64;

	struct EventInfo {
		unsigned long timer;
		std::string file;
		int line;
	};

	std::map<unsigned int, EventInfo> id_to_info;

	#ifdef ESP32
		#include "freertos/semphr.h"
		static SemaphoreHandle_t mutex;  // Semaphore for thread safety
	#endif

	static unsigned int get_id(const std::string& file, int line);
	static bool trigger_id(int ms, unsigned int event_id);
};

#define SYNC_EVENT(ms) SmartSyncEvent::trigger(ms, __FILE__, __LINE__)

#endif // ___SMART_SYNC_EVENT_H___
