add_library(uart_lib STATIC uart_hal.c uart_mgr.c)
target_include_directories(uart_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/../common
)
