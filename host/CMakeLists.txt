cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)

set(SOURCES
    src/main.c
    src/port.c
)

include_directories(include)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE c)

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    COMMENT "Running ${PROJECT_NAME} executable"
)
