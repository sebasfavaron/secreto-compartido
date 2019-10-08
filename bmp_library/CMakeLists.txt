add_library(
        bmp_library
        src/bmp.h
        src/bmp.c
)

target_include_directories(bmp_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")