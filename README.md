# 🌱 Smart Farm IoT System (Telemetry Architecture)

⚠️ **Current Status:** The Internet/Cloud integration (WiFi provisioning and Adafruit IO MQTT syncing) is currently under active development and is not yet ready for production.

---

## 📁 Project Structure & File Roles

The codebase is strictly divided into the PC control software (`/system-control`) and the microcontroller firmware (`/src` and `/include`).

```text
SmartFarm_Project/
├── .env                  # User-specific secret credentials (Git-ignored)
├── build_env.py          # PlatformIO pre-build script to inject .env secrets into C++
├── platformio.ini        # PlatformIO build and dependency configuration
│
├── system-control/       # 🖥️ PC DASHBOARD (Python)
│   ├── main.py           # The main Python TUI dashboard using the 'rich' library. Reads JSON from Serial.
│   └── requirements.txt  # Python package dependencies (pyserial, rich, python-dotenv).
│
├── include/              # 🧠 FIRMWARE HEADERS (C++)
│   ├── config.h          # Hardware pin definitions and timing configurations.
│   ├── sensors.h         # Definitions for DHT22, Soil Moisture, and LDR interfaces.
│   ├── relay.h           # Definitions for water pump relay logic.
│   ├── device.h          # Definitions for MAC address utility.
│   ├── mqtt-client.h     # Definitions for Adafruit IO publishing.
│   └── wifi-manager.h    # Definitions for local network connections.
│
└── src/                  # 🧠 ESP32 FIRMWARE SOURCE (C++)
    ├── main.cpp          # The main loop: orchestrates sensor reads and serializes data into JSON.
    ├── sensors.cpp       # Hardware implementations and Lux calculations.
    ├── relay.cpp         # Active-LOW logic implementation for the water pump relay.
    ├── device.cpp        # Utility to fetch the ESP32's hardware MAC address.
    ├── mqtt-client.cpp   # [WIP] Module for publishing telemetry to Adafruit IO.
    └── wifi-manager.cpp  # [WIP] Module for handling local network connections.