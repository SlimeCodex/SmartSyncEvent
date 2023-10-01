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

#include "SmartSyncEvent.h"

std::map<unsigned int, unsigned long> SmartSyncEvent::id_to_timer;
SemaphoreHandle_t SmartSyncEvent::mutex = xSemaphoreCreateMutex();

SmartSyncEvent::SmartSyncEvent() {
}

SmartSyncEvent::~SmartSyncEvent() {
    vSemaphoreDelete(mutex);
}

unsigned int SmartSyncEvent::get_id(const std::string& file, int line) {
    unsigned int hash = 0;
    for (char c : file) {
        hash = (hash * 31) + c;
    }
    return hash + line;
}

bool SmartSyncEvent::trigger_id(int ms, unsigned int eventID) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    unsigned long& timer = id_to_timer[eventID];

    if (id_to_timer.size() > MAX_INSTANCES) {
        xSemaphoreGive(mutex);
        return false;
    }

    bool shouldTrigger = (millis() - timer > ms);
    if (shouldTrigger) {
        timer = millis();
    }

    xSemaphoreGive(mutex);
    return shouldTrigger;
}

SmartSyncEvent::Result SmartSyncEvent::trigger(int ms, const std::string& file, int line) {
    unsigned int eventID = get_id(file, line);
    bool wasTriggered = trigger_id(ms, eventID);
    return {wasTriggered, eventID};
}

void SmartSyncEvent::reset(unsigned int eventID) {
    xSemaphoreTake(mutex, portMAX_DELAY);
    if (id_to_timer.find(eventID) != id_to_timer.end()) {
        id_to_timer[eventID] = millis();
    }
    xSemaphoreGive(mutex);
}