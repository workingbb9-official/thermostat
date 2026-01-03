cmake_minimum_required(VERSION 3.15)
project(firmware LANGUAGES C)

function(create_lib name source)
    set(multiValueArgs INCLUDE_DIRS LINK_LIBS)
    cmake_parse_arguments(LIB "" "" "${multiValueArgs}" ${ARGN})

    add_library(${name} ${source})

    target_include_directories(${name} PUBLIC include)
    if(LIB_INCLUDE_DIRS)
        target_include_directories(${name} PRIVATE ${LIB_INCLUDE_DIRS})
    endif()

    if(LIB_LINK_LIBS)
        target_link_libraries(${name} PRIVATE ${LIB_LINK_LIBS})
    endif()
endfunction()

# drivers
create_lib(therm_driver src/drivers/thermistor.c)
create_lib(uart_driver src/drivers/uart.c)
create_lib(lcd_driver src/drivers/lcd.c)

# logic
create_lib(therm_mgr src/logic/therm_mgr.c
    LINK_LIBS therm_driver)
create_lib(uart_mgr src/logic/uart_mgr.c
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
    LINK_LIBS uart_driver)
create_lib(lcd_mgr src/logic/lcd_mgr.c
    LINK_LIBS lcd_driver)

# system 
create_lib(system_core src/app/system_core.c
    LINK_LIBS therm_mgr uart_mgr lcd_mgr
)
create_lib(system_temperature src/app/system_temperature.c
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
    LINK_LIBS therm_mgr uart_mgr lcd_mgr
)

function(add_avr_firmware TARGET_NAME SOURCE_FILES)
    add_executable(${TARGET_NAME} ${SOURCE_FILES})
    
    # Generate Hex
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom $<TARGET_FILE:${TARGET_NAME}> ${TARGET_NAME}.hex
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${TARGET_NAME}>
    )

    # Create a specific upload target for this test (e.g., make upload_test_lcd)
    add_custom_target(upload_${TARGET_NAME}
        COMMAND avrdude -v -p atmega328p -c arduino -P /dev/ttyACM0 -b 115200 -U flash:w:${TARGET_NAME}.hex:i
        DEPENDS ${TARGET_NAME}
        COMMENT "Uploading ${TARGET_NAME}.hex to Arduino"
    )
endfunction()

if (TESTS)
    add_subdirectory(tests)
endif()

add_avr_firmware(firmware src/main.c)
target_link_libraries(firmware PRIVATE system_core system_temperature)
