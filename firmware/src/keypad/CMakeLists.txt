add_library(keypad_lib STATIC keypad_hal.c keypad.c)
target_include_directories(keypad_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
