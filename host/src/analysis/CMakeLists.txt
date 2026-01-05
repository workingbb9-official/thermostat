add_library(analysis_lib STATIC analysis.c)
target_include_directories(analysis_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)
