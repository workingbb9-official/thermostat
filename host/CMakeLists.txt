cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)
option(TESTS OFF)

add_subdirectory(src/storage)
add_subdirectory(src/analysis)
add_subdirectory(src/port)
add_subdirectory(src/app)
