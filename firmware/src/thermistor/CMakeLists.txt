add_library(therm_lib STATIC therm_hal.c thermistor.c)
target_include_directories(therm_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
