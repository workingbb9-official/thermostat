add_library(analysis_lib STATIC src/analysis.c)
target_include_directories(analysis_lib
    PUBLIC include
    PRIVATE src
)

if (TESTS)
    add_executable(test_analysis tests/test_analysis.c
        src/analysis.c
    )
    target_include_directories(test_analysis
        PUBLIC include
    )
endif()
