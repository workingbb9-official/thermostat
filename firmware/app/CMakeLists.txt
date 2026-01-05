add_executable(thermostat
    main.c
    src/system_core.c
    src/system_display.c
    src/system_temperature.c
    src/system_data_handler.c
)

target_include_directories(thermostat 
    PRIVATE include
    PRIVATE ../..
)

target_link_libraries(thermostat
    PRIVATE 
    lcd_lib therm_lib uart_lib
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${PROJECT_NAME}> ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
)

add_custom_target(upload_${PROJECT_NAME}
    COMMAND avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:${PROJECT_NAME}.hex:i
    DEPENDS ${PROJECT_NAME}
    COMMENT "Uploading ${PROJECT_NAME}.hex to Arduino"
)
