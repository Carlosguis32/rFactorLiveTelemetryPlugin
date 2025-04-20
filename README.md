# 🏎️ rFactor Live Telemetry Plugin

A powerful C++ plugin that streams real-time telemetry data from rFactor via UDP. Get instant access to all available simulator signals for advanced race analysis and monitoring.

## 🌟 Features

-   📊 **Real-time Data Streaming**: Continuous transmission of all simulator signals
-   🌐 **UDP Communication**: Flexible data transmission over network
-   🏃 **Multi-Driver Support**: Track data from all drivers in the session
-   ⚡ **Configurable Performance**: Adjustable transmission frequency
-   🔌 **Easy Integration**: Simple plugin installation process

## 📋 Prerequisites

-   rFactor Racing Simulator
-   Visual Studio (for building from source)

## 🔧 Technologies

-   **Language**: C++
-   **Network Protocol**: UDP
-   **Data Format**: Binary Packets
-   **Configuration**: JSON

## 💻 Installation

1. Locate your rFactor installation directory
2. Copy the following files to the `Plugins` folder, located inside rFactor's root folder:
    - `TelemetryPlugin.dll`
    - `telemetryConfig.json`

## ⚙️ Configuration

Modify `telemetryConfig.json` to customize the plugin:

-   **IP**: Target address for data transmission
-   **port**: UDP port for communication
-   **frequency**: Maximum data transmission rate

> The rest of the fields on the JSON file are purely informative

## 📦 Data Structure

The plugin transmits data in packets, with packet_3 containing variable-size driver data:

-   Each driver's data block: 428 bytes
-   Multiple drivers supported (e.g. 3 drivers = 1284 bytes)
-   Multiple IP and ports supported, just input them in the JSON file (e.g. `"ip": ["127.0.0.1", "192.13.16.81"], "port": ["6000", "8135"]`)
-   Data format: [size in bytes, type]
-   Empty bytes marked as "skip" type

Your driver's data is always contained in the first 428 bytes of packet_3, followed by other drivers' data.

## 🏗️ Building from Source

The project is built using Visual Studio with standard C++ libraries.
