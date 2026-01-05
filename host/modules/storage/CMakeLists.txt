add_library(storage_lib STATIC src/storage_mgr.c src/file_utils.c)
target_include_directories(storage_lib
    PUBLIC include
    PRIVATE src
)
