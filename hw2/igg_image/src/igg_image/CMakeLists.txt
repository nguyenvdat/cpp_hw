#################### Import the 'io_tools' library here ########################
find_library(TOOLS
			 NAMES libio_tools.dylib
			 PATHS ${LIBRARY_OUTPUT_PATH})

#################### Create a library 'image' below this line ##################
add_library(image image.cpp)
target_link_libraries(image "${TOOLS}")
add_executable(bin_main main.cpp)
target_link_libraries(bin_main 
                      image)
