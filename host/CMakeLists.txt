cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)

set(PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
option(TESTS OFF)

# serial port
add_library(serial_port src/services/port.c)
add_library(port_mgr src/logic/port_mgr.c)

target_include_directories(serial_port
    PUBLIC include
)
target_include_directories(port_mgr
    PUBLIC include
)

target_link_libraries(port_mgr PRIVATE serial_port)

# file utils
add_library(file_utils src/services/file_utils.c)
target_include_directories(file_utils
    PUBLIC include
)

# analysis
add_library(data_analysis src/logic/analysis.c)
target_include_directories(data_analysis
    PUBLIC include
)

if (TESTS)
    add_subdirectory(tests)
endif()

add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE port_mgr file_utils data_analysis)

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running ${PROJECT_NAME} executable"
)
