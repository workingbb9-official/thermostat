cmake_minimum_required(VERSION 3.15)
project(firmware LANGUAGES C)

# thermistor
add_library(therm_driver src/drivers/thermistor.c)
add_library(therm_mgr src/logic/therm_mgr.c)

target_include_directories(therm_driver
    PUBLIC include
)
target_include_directories(therm_mgr
    PUBLIC include
)
target_link_libraries(therm_mgr PRIVATE therm_driver)

# UART
add_library(uart_driver src/drivers/uart.c)
add_library(uart_mgr src/logic/uart_mgr.c)

target_include_directories(uart_driver
    PUBLIC include
)
target_include_directories(uart_mgr
    PUBLIC include
    PRIVATE ${CMAKE_SOURCE_DIR}/..
)
target_link_libraries(uart_mgr PRIVATE uart_driver)

# system 
add_library(system_core src/app/system_core.c)

target_include_directories(system_core
    PUBLIC include
)
target_link_libraries(system_core PRIVATE therm_mgr uart_mgr)

add_library(system_temperature src/app/system_temperature.c)

target_include_directories(system_temperature
    PUBLIC include
    PRIVATE ${CMAKE_SOURCE_DIR}/..
)
target_link_libraries(system_temperature PRIVATE therm_mgr uart_mgr)


add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE system_core system_temperature)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    # Use $<TARGET_FILE:...> to resolve the full path to Keypad.elf
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    # Report size using avr-size
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    COMMENT "Generated ${PROJECT_NAME}.hex and reported size."
)

add_custom_target(upload
	COMMAND avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:${PROJECT_NAME}.hex:i
	DEPENDS ${PROJECT_NAME}
	COMMENT "Uploading ${PROJECT_NAME}.hex to Arduino uno."
)
