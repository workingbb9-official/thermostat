add_executable(host
    src/main.c
    src/system_core.c
    src/system_data_handler.c
    src/system_data_receiver.c
)
target_include_directories(host
    PUBLIC include
    PRIVATE ../..
)
target_link_libraries(host
    PRIVATE
    storage_lib analysis_lib port_lib
)
