# SmartSyncEvent Library

The `SmartSyncEvent` library provides a convenient way to manage timed events in Arduino and ESP32 projects, enabling you to check whether a certain amount of time has passed since the last event trigger.

## Features

- **Automatic Instance Management**: Manages unique event instances based on the file and line of invocation.
- **Thread Safety**: Provides thread-safe handling for ESP32 projects.
- **Convenience**: Utilize through a simple macro for triggering events.

## Installation using PlatformIO

PlatformIO offers a streamlined way to manage your project dependencies.

1. Open your platformio.ini file in the root of your project.
2. Add the following line under your environment's lib_deps:

```
lib_deps =
    https://github.com/SlimeCodex/SmartSyncEvent@^1.3.0
```

3. Save the platformio.ini file.
4. Build your project. PlatformIO will automatically fetch and install the SmartSyncEvent library.

## Basic Usage

### Including the Library

```cpp
#include "SmartSyncEvent.h"
```

### Triggering an Event

Utilize the `SYNC_EVENT(ms)` macro. This returns a Result struct, which can be implicitly cast to bool to verify if the event was triggered. Moreover, the event's unique ID can be accessed with the .eventID member.

```cpp
if (SYNC_EVENT(500)) {
    // This block will execute every 500ms.
}
```

## Advanced Usage

### Retrieving the Event ID Directly

Directly obtain a unique ID for an event:

```cpp
auto result = SYNC_EVENT(1000);
unsigned int event_id = result.eventID;
```

### Resetting an Event Timer

For manual timer resets of any sync event:

```cpp
SmartSyncEvent::reset(event_id);
```

### Enable an Event

To enable a previously disabled event:

```cpp
SmartSyncEvent::enable(event_id);
```

### Disable an Event

To disable a previously enabled event:

```cpp
SmartSyncEvent::disable(event_id);
```

### Forcing an Event to Trigger

The force method provides a way to manually enforce an event to trigger the next time it's checked, regardless of the time interval set during its creation.

```cpp
SmartSyncEvent::force(event_id);
```

## License

Distributed under the GPL-3.0 License. See the LICENSE file for more information.