# Thermostat - Simple environmental monitoring and logging system written in C

## Features
* **Indoor Temp:** Uses thermistor to calculate temperature and displays on lcd
* **Weather:** Uses HTTP with a web API to get current outdoor temp and weather condition
* **Login:** Need to input password to get into system, compared against host database
* **Storage:** Stores indoor temperature and session details (login and logout time) on host
* **Statistics:** Uses the temperature storage to calculate average, max, and min

## Architecture
* **MCU/Firmware:** Contains thermistor, LCD, keypad, and UART components
* **Host PC:** Handles networking, storage, and login authorization

## Implementation
* **State Machine:** Firmware has 3 states: login, home, and stats
* **File i/o:** Uses POSIX calls to write and read from storage files
* **Data transfer:** Information flows through UART (firmware) and serial port (host)
* **Networking:** Weather data is fetched using sockets and HTTP requests

## Hardware
* **Microcontroller:** ATmega328P (Arduino Uno) @ 16MHz
* **LCD:** HD44780 16x2 in 4-bit mode
* **Sensor:** 10k NTC Thermistor
* **Keypad:** 4x4 Matrix Membrane
* **Communication:** UART via USB-Serial (115200 Baud)
