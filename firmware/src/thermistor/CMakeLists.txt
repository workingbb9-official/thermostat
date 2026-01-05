add_library(therm_lib STATIC therm_hal.c therm_mgr.c)
target_include_directories(therm_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
