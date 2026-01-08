add_executable(host
    main.c
    system_core.c
)
target_include_directories(host
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${CMAKE_SOURCE_DIR}/..
)
target_link_libraries(host
    PRIVATE
    storage_lib analysis_lib port_lib
)
