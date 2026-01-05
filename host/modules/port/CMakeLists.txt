add_library(port_lib STATIC src/port_hal.c src/port_mgr.c)
target_include_directories(port_lib
    PUBLIC include
    PRIVATE src
)
