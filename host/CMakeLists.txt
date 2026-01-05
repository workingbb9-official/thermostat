cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)
option(TESTS OFF)

add_subdirectory(modules/storage)
add_subdirectory(modules/analysis)
add_subdirectory(modules/port)
add_subdirectory(app)
