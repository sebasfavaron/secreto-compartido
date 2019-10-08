add_library(
        matrix_library
        libraryMod251.h
        libraryMod251.c
        matrixLibrary.h
        matrixLibrary.c
)

target_include_directories(matrix_library PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")