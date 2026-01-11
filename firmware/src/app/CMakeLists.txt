add_executable(firmware
    main.c
    system_core.c
    state_login.c
    state_home.c
)

target_include_directories(firmware
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/../common
)

target_link_librariesfirmware(
    PRIVATE 
    lcd_lib therm_lib uart_lib keypad_lib
)

add_firmware(firmware)
