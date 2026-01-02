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

# logic
create_lib(therm_mgr src/logic/therm_mgr.c
    LINK_LIBS therm_driver)
create_lib(uart_mgr src/logic/uart_mgr.c
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
    LINK_LIBS uart_driver)

# system 
create_lib(system_core src/app/system_core.c
    LINK_LIBS therm_mgr uart_mgr
)
create_lib(system_temperature src/app/system_temperature.c
    INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
    LINK_LIBS therm_mgr uart_mgr
)

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE system_core system_temperature)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
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
