add_library(digit_counting digit_counting.cpp)
add_executable(bin_main main.cpp)
target_link_libraries(bin_main 
                      digit_counting)
