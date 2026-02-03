add_library(weather_lib STATIC weather.c cJSON.c)
target_include_directories(weather_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/../common
)

if (TESTS)
    add_executable(test_weather test_weather.c)
    target_include_directories(test_weather
        PUBLIC ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(test_weather PRIVATE weather_lib)
endif()
