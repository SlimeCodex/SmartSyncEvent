/**
 * Copyright (C) 2023 Alejandro Nicolini
 * 
 * This file is part of SmartSyncEvent.
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
 * 
**/

#ifndef ___SMART_SYNC_EVENT_H___
#define ___SMART_SYNC_EVENT_H___

#include <Arduino.h>
#include <string>
#include <map>

class SmartSyncEvent {
private:
	static const int MAX_INSTANCES = 64;

	static std::map<unsigned int, unsigned long> id_to_timer;

	#ifdef ESP32
		#include "freertos/semphr.h"
		static SemaphoreHandle_t mutex;  // Semaphore for thread safety
	#endif

	static unsigned int get_id(const std::string& file, int line);
	static bool trigger_id(int ms, unsigned int eventID);

public:
	SmartSyncEvent();
	~SmartSyncEvent();

	struct Result {
		bool triggered;
		unsigned int eventID;

		// User-defined conversion to bool
		operator bool() const {
			return triggered;
		}
	};

	static Result trigger(int ms, const std::string& file, int line);
	static void reset(unsigned int eventID);
};

#define SYNC_EVENT(ms) SmartSyncEvent::trigger(ms, __FILE__, __LINE__)

#endif // ___SMART_SYNC_EVENT_H___