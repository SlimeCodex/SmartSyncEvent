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

#include "SmartSyncEvent.h"

std::map<unsigned int, unsigned long> SmartSyncEvent::id_to_timer;
std::map<unsigned int, bool> SmartSyncEvent::id_to_status;

#ifdef ESP32
	SemaphoreHandle_t SmartSyncEvent::mutex = xSemaphoreCreateMutex();
#endif

SmartSyncEvent::SmartSyncEvent() {
}

SmartSyncEvent::~SmartSyncEvent() {
	#ifdef ESP32
		vSemaphoreDelete(mutex); // Cleanup for ESP32
	#endif
}

unsigned int SmartSyncEvent::get_id(const std::string& file, int line) {
	unsigned int hash = 0;
	for (char c : file) {
		hash = (hash * 31) + c;
	}
	return hash + line;
}

bool SmartSyncEvent::trigger_id(int ms, unsigned int event_id) {
	#ifdef ESP32
		xSemaphoreTake(mutex, portMAX_DELAY);
	#endif

	// Check enabled or disabled event
	if (id_to_status.find(event_id) != id_to_status.end() && !id_to_status[event_id]) {
		#ifdef ESP32
			xSemaphoreGive(mutex);
		#endif
		return false;
	}

	unsigned long& timer = id_to_timer[event_id];

	if (id_to_timer.size() > MAX_INSTANCES) {
		#ifdef ESP32
			xSemaphoreGive(mutex);
			ESP_LOGW("SmartSyncEvent", "Too many unique sync events instances. Consider expanding MAX_INSTANCES buffer.");
		#endif
		return false;
	}

	bool trigger_status = (millis() - timer > ms);
	if (trigger_status) {
		timer = millis();

		#ifdef ESP32
			ESP_LOGV("SmartSyncEvent", "Event with ID: %X triggered", event_id);
		#endif
	}

	#ifdef ESP32
		xSemaphoreGive(mutex);
	#endif

	return trigger_status;
}

SmartSyncEvent::Result SmartSyncEvent::trigger(int ms, const std::string& file, int line) {
	unsigned int event_id = get_id(file, line);
	bool wasTriggered = trigger_id(ms, event_id);
	return {wasTriggered, event_id};
}

// Generic helper function (reset, enable, disable and force)
bool SmartSyncEvent::modify_event(unsigned int event_id, std::function<bool()> action, const char* logMessage) {
	#ifdef ESP32
		xSemaphoreTake(mutex, portMAX_DELAY);
	#endif

	// Execute the unique action
	bool success = action();

	if (!success) {
		#ifdef ESP32
			ESP_LOGV("SmartSyncEvent", "Unable to find event with ID: %X", event_id);
			xSemaphoreGive(mutex);
		#endif
		return false;
	}

	#ifdef ESP32
		ESP_LOGV("SmartSyncEvent", logMessage, event_id);
		xSemaphoreGive(mutex);
	#endif
	return true;
}

// Using the helper function to simplify reset, enable, disable and force
void SmartSyncEvent::reset(unsigned int event_id) {
	modify_event(event_id, [&]() -> bool {
		if (id_to_timer.find(event_id) == id_to_timer.end()) return false;
		id_to_timer[event_id] = millis();
		return true;
	}, "Event with ID: %X has been reset");
}

void SmartSyncEvent::disable(unsigned int event_id) {
	modify_event(event_id, [&]() -> bool {
		if (id_to_status.find(event_id) == id_to_status.end()) return false;
		id_to_status[event_id] = false;
		return true;
	}, "Event with ID: %X has been disabled");
}

void SmartSyncEvent::enable(unsigned int event_id) {
	modify_event(event_id, [&]() -> bool {
		if (id_to_status.find(event_id) == id_to_status.end()) return false;
		id_to_status[event_id] = true;
		return true;
	}, "Event with ID: %X has been enabled");
}

void SmartSyncEvent::force(unsigned int event_id) {
	modify_event(event_id, [&]() -> bool {
		if (id_to_timer.find(event_id) == id_to_timer.end()) return false;
		id_to_timer[event_id] = 0;
		return true;
	}, "Event with ID: %X has been forced");
}