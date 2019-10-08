add_library(
        image_secret
        src/secret_workflow.h src/secret_workflow.c
        src/matrix_wrapper.c src/matrix_wrapper.h)

target_include_directories(image_secret PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_link_libraries(image_secret matrix_library)
target_link_libraries(image_secret bmp_library)
target_link_libraries(image_secret m)