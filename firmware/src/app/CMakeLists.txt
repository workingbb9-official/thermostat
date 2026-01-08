add_executable(thermostat
    main.c
    system_core.c
)

target_include_directories(thermostat
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/..
)

target_link_libraries(thermostat
    PRIVATE 
    lcd_lib therm_lib uart_lib keypad_lib
)

add_firmware(thermostat)
