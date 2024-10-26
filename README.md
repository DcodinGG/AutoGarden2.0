# AutoGarden 2.0

AutoGarden 2.0 is an automated gardening system that allows you to monitor and control plant irrigation through soil moisture sensors and a DHT22 temperature/humidity sensor. This project utilizes an ESP32 for data collection and irrigation management, and allows data to be stored in a MySQL database.

## Features

- Monitor soil moisture levels for multiple plants.
- Read ambient temperature and humidity.
- Automatic irrigation based on soil moisture levels.
- Web interface to visualize real-time data.
- Data storage in a MySQL database.

## Components

- **ESP32**: Microcontroller running the system.
- **Soil Moisture Sensors**: For measuring soil moisture levels.
- **DHT22**: Temperature and humidity sensor.
- **Solenoid Valves**: For controlling plant irrigation.
- **MySQL Database**: For storing system data.

## Requirements

- **Hardware**:
  - ESP32
  - Soil moisture sensors
  - DHT22
  - Solenoid valves
  - Necessary cables and connections

- **Software**:
  - Arduino IDE or PlatformIO
  - Required libraries:
    - DHT sensor library
    - ESP32_MySQL

## Installation

1. Clone this repository to your local machine:
   ```bash
   git clone https://github.com/DcodinGG/AutoGarden2.0.git
2. Open the project in Arduino IDE or PlatformIO.

3. Install the necessary libraries (DHT sensor library and ESP32_MySQL) if you haven't done so already.

4. Configure your WiFi and database credentials in credentials.h.

5. Upload the code to the ESP32.

## Usage
Connect all components according to the project schematic.
Ensure the ESP32 is connected to the WiFi network.
The system will start monitoring moisture levels and irrigating automatically based on the defined settings.