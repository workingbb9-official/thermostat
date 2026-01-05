add_library(analysis_lib STATIC src/analysis.c)
target_include_directories(analysis_lib
    PUBLIC include
    PRIVATE src
)
