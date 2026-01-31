add_library(file_lib STATIC file_utils.c) 
target_include_directories(file_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
