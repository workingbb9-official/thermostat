cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)
option(TESTS OFF)

add_subdirectory(src/fileio)
add_subdirectory(src/analysis)
add_subdirectory(src/port)
add_subdirectory(src/network)
add_subdirectory(src/weather)

add_executable(host
    src/main.c
    src/app/sys_core.c
    src/app/session.c
    src/app/state_home.c
    src/app/state_stats.c
    src/app/weather_client.c
)
target_include_directories(host
    PUBLIC include
    PRIVATE src/app
    PRIVATE ../common
)
target_link_libraries(host
    PRIVATE
    file_lib analysis_lib port_lib weather_lib network_lib
)
