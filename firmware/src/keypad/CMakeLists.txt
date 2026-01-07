add_library(keypad_lib STATIC keypad.c)
target_include_directories(keypad_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

if (TESTS)
    add_executable(test_keypad_driver 
        tests/test_keypad_driver.c
        keypad.c
    )

    target_include_directories(test_keypad_driver
        PUBLIC ${CMAKE_SOURCE_DIR}/include
    )

    target_link_libraries(test_keypad_driver
        PRIVATE lcd_lib
    )

    add_firmware(test_keypad_driver)
endif()
