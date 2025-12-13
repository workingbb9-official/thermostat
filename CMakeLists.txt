set(CMAKE_TOOLCHAIN_FILE
	"${CMAKE_CURRENT_SOURCE_DIR}/avr.toolchain.cmake"
	CACHE INTERNAL ""
)

cmake_minimum_required(VERSION 3.15)

project(read_temp LANGUAGES C)

set(SOURCES
	src/main.c
	src/thermistor.c
	src/uart.c
)

include_directories(include)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE m c printf_flt)

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
