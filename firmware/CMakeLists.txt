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

add_subdirectory(modules/thermistor)
add_subdirectory(modules/uart)
add_subdirectory(modules/lcd)
add_subdirectory(app)
