add_library(storage_lib STATIC src/storage_mgr.c src/file_utils.c)
target_include_directories(storage_lib
    PUBLIC include
    PRIVATE src
)

if (TESTS)
    add_executable(test_file_utils tests/test_file_utils.c
        src/file_utils.c
    )
    target_include_directories(test_file_utils
        PRIVATE src
    )
endif()
