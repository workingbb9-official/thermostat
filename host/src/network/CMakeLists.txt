add_library(network_lib STATIC network.c)
target_include_directories(network_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

if (TESTS)
    add_executable(test_http test_http.c)
    target_include_directories(test_http
        PUBLIC ${CMAKE_SOURCE_DIR}/include
    )
    target_link_libraries(test_http PRIVATE network_lib)
endif()
