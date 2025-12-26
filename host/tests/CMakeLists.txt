cmake_minimum_required(VERSION 3.15)
project(test LANGUAGES C)

add_executable(test_file_utils services/test_file_utils.c)
target_link_libraries(test_file_utils PRIVATE file_utils)
set_target_properties(test_file_utils PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_executable(test_analysis logic/test_analysis.c)
target_link_libraries(test_analysis PRIVATE data_analysis)
set_target_properties(test_analysis PROPERTIES EXCLUDE_FROM_ALL TRUE)
