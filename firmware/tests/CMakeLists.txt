cmake_minimum_required(VERSION 3.15)
project(test LANGUAGES C)

# LCD
add_avr_firmware(test_lcd_driver test_lcd_driver.c)
target_link_libraries(test_lcd_driver PRIVATE lcd_driver)
