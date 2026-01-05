add_library(lcd_lib STATIC src/lcd_hal.c src/lcd_mgr.c)
target_include_directories(lcd_lib
    PUBLIC include
    PRIVATE src
    PRIVATE ..
)
