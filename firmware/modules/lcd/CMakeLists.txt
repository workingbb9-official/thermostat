add_library(lcd_lib STATIC src/lcd_hal.c src/lcd_mgr.c)
target_include_directories(lcd_lib
    PUBLIC include
    PRIVATE src
    PRIVATE ..
)

if (TESTS)
    add_executable(test_lcd_driver tests/test_lcd_driver.c)
    target_include_directories(test_lcd_driver
        PRIVATE src
        PRIVATE ..
    )
    add_firmware(test_lcd_driver)

    add_executable(test_lcd_mgr tests/test_lcd_mgr.c)
    target_link_libraries(test_lcd_mgr PRIVATE lcd_lib)
    add_firmware(test_lcd_mgr)
endif()
