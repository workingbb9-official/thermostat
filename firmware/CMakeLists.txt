cmake_minimum_required(VERSION 3.15)
project(thermostat LANGUAGES C)
option(TESTS OFF)

function(add_firmware target_name)
    add_custom_command(
        TARGET ${target_name} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${target_name}> ${target_name}.hex
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${target_name}>
    )

    add_custom_target(upload_${target_name}
        COMMAND avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:${target_name}.hex:i
        DEPENDS ${target_name}
        COMMENT "Uploading ${target_name}.hex to Arduino"
    )
endfunction()

add_subdirectory(src/thermistor)
add_subdirectory(src/uart)
add_subdirectory(src/lcd)
add_subdirectory(src/keypad)

add_executable(thermostat
    src/main.c
    src/app/system_core.c
    src/app/state_login.c
    src/app/state_home.c
    src/app/state_stats.c
)
target_include_directories(thermostat
    PUBLIC include
    PRIVATE ../common
    PRIVATE src/app
)
target_link_libraries(thermostat
    PRIVATE
    therm_lib uart_lib lcd_lib keypad_lib
)

add_firmware(thermostat)
