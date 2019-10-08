add_library(
        steganography
        steg.c
        steg.h
)

target_include_directories(steganography PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(steganography bmp_library)
