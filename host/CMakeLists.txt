cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)
option(TESTS OFF)

add_subdirectory(src/storage)
add_subdirectory(src/analysis)
add_subdirectory(src/port)
add_subdirectory(src/network)
add_subdirectory(src/weather)

add_executable(host
    src/main.c
    src/app/system_core.c
    src/app/sys_utils.c
)
target_include_directories(host
    PUBLIC include
    PRIVATE src/app
    PRIVATE ../common
)
target_link_libraries(host
    PRIVATE
    storage_lib analysis_lib port_lib
)
