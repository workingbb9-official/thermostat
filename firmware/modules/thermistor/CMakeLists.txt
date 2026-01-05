add_library(therm_lib STATIC src/therm_hal.c src/therm_mgr.c)
target_include_directories(therm_lib
    PUBLIC include
    PRIVATE src
    PRIVATE ..
)
