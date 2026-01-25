add_library(lcd_lib STATIC lcd_hal.c lcd.c)
target_include_directories(lcd_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
