# SmartSyncEvent Library

The `SmartSyncEvent` library provides a convenient way to manage timed events in Arduino and ESP32 projects, enabling you to check whether a certain amount of time has passed since the last event trigger.

## Features

- **Automatic Instance Management**: Manages unique event instances based on the file and line of invocation.
- **Thread Safety**: Provides thread-safe handling for ESP32 projects.
- **Convenience**: Utilize through a simple macro for triggering events.

## Installation

1. Download the library as a zip file.
2. Open your Arduino IDE or PlatformIO.
3. Add the library via the respective "Add Library" feature.

## Usage

### Including the Library

```cpp
#include "SmartSyncEvent.h"
```

### Triggering an Event

Utilize the SYNC_EVENT(ms) macro. This returns a Result struct, which can be implicitly cast to bool to verify if the event was triggered. Moreover, the event's unique ID can be accessed with the .eventID member.

```cpp
if (SYNC_EVENT(500)) {
    // This block will execute every 500ms.
}
```

## Manually Resetting an Event Timer

For manual timer resets of any sync event, use its unique ID.

```cpp
SmartSyncEvent::reset(eventID);
```

## Retrieving the Event ID Directly

Directly obtain a unique ID for an event:

```cpp
auto result = SYNC_EVENT(1000);
unsigned int id = result.eventID;
```

## License

Distributed under the MIT License. See the LICENSE file for more information.