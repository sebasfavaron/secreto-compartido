cmake_minimum_required(VERSION 3.10)
project(cripto_secreto_compartido C)

set(CMAKE_C_STANDARD 99)

add_subdirectory(bmp_library)
add_subdirectory(matrix_library)
add_subdirectory(image_secret)
add_subdirectory(integration_module)
add_subdirectory(steganography)