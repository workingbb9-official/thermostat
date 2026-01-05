cmake_minimum_required(VERSION 3.15)
project(host LANGUAGES C)
# 
# option(TESTS OFF)
# 
# function(create_lib name source)
#     set(multiValueArgs INCLUDE_DIRS LINK_LIBS)
#     cmake_parse_arguments(LIB "" "" "${multiValueArgs}" ${ARGN})
# 
#     add_library(${name} ${source})
# 
#     target_include_directories(${name} PUBLIC include)
#     if(LIB_INCLUDE_DIRS)
#         target_include_directories(${name} PRIVATE ${LIB_INCLUDE_DIRS})
#     endif()
# 
#     if(LIB_LINK_LIBS)
#         target_link_libraries(${name} PRIVATE ${LIB_LINK_LIBS})
#     endif()
# endfunction()
# 
# # services
# create_lib(serial_port src/services/port.c)
# create_lib(file_utils src/services/file_utils.c)
# 
# # logic
# create_lib(port_mgr src/logic/port_mgr.c
#     LINK_LIBS serial_port
# )
# create_lib(storage_mgr src/logic/storage_mgr.c
#     LINK_LIBS file_utils
# )
# create_lib(data_analysis src/logic/analysis.c)
# 
# # system
# create_lib(system_data_receiver src/app/system_data_receiver.c
#     INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
#     LINK_LIBS port_mgr
# )
# create_lib(system_data_handler src/app/system_data_handler.c
#     INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
#     LINK_LIBS storage_mgr
# )
# create_lib(system_core src/app/system_core.c
#     INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/..
#     LINK_LIBS 
#     system_data_receiver system_data_handler 
#     port_mgr storage_mgr data_analysis
# )
# 
# if (TESTS)
#     add_subdirectory(tests)
# endif()
# 
# add_executable(host src/main.c)
# target_link_libraries(host PRIVATE system_core)
# 
# add_custom_target(run
#     COMMAND host
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/..
#     DEPENDS host
#     COMMENT "Running host executable"
# )

add_subdirectory(modules/storage)
add_subdirectory(modules/analysis)
add_subdirectory(modules/port)
add_subdirectory(app)
