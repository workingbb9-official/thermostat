add_library(storage_lib STATIC file_utils.c file_mgr.c)
target_include_directories(storage_lib
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)
