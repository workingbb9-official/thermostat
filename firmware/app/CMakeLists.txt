add_executable(thermostat
    src/main.c
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

add_firmware(thermostat)
