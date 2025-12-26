cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

set(SOURCES
    src/main.c
    src/port.c
    src/analysis.c
    src/file_utils.c
)

include_directories(include)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE c)

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    WORKING_DIRECTORY ${PROJECT_ROOT_DIR}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running ${PROJECT_NAME} executable"
)
