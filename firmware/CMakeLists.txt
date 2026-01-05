cmake_minimum_required(VERSION 3.15)
project(thermostat LANGUAGES C)

add_subdirectory(modules/thermistor)
add_subdirectory(modules/uart)
add_subdirectory(modules/lcd)
add_subdirectory(app)
