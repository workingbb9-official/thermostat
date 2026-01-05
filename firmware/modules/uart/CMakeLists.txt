add_library(uart_lib STATIC src/uart_hal.c src/uart_mgr.c)
target_include_directories(uart_lib
    PUBLIC include
    PRIVATE src
    PRIVATE ..
    PRIVATE ../../..
)
