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
 * along with SmartSyncEvent.  If not, see <https://www.gnu.org/licenses/>.
 * 
**/

#ifndef ___SMART_SYNC_EVENT_H___
#define ___SMART_SYNC_EVENT_H___

#include <Arduino.h>
#include <freertos/semphr.h> // For the semaphore on ESP32
#include <string>
#include <map>

class SmartSyncEvent {
public:
    struct Result {
        bool triggered;
        unsigned int eventID;

        // User-defined conversion to bool
        operator bool() const {
            return triggered;
        }
    };

    SmartSyncEvent();
    ~SmartSyncEvent();

    static Result trigger(int ms, const std::string& file, int line);
    static void reset(unsigned int eventID);

private:
    static const int MAX_INSTANCES = 64;
    static std::map<unsigned int, unsigned long> id_to_timer;
    static SemaphoreHandle_t mutex;

    static unsigned int get_id(const std::string& file, int line);
    static bool trigger_id(int ms, unsigned int eventID);
};

// Helper
#define SYNC_EVENT(ms) SmartSyncEvent::trigger(ms, __FILE__, __LINE__)

#endif // ___SMART_SYNC_EVENT_H___