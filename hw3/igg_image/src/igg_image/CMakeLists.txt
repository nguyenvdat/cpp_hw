add_subdirectory(io_strategies)

# TODO: create image library from image.cpp.
# TODO: link image library to any other libraries if needed.
add_library(image image.cpp)
target_link_libraries(image strategies)
add_executable(bin_main main.cpp)
target_link_libraries(bin_main 
                      image)
