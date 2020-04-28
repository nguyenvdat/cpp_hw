# If you are not on Linux, you will need to download the source of the
# googletest framework manually and provide the folder to which you have
# - downloaded it by copying this link into your browser:
#   https://github.com/google/googletest/archive/release-1.8.0.zip
# - unpack it into some <FOLDER>
# - use this <FOLDER> instead of /usr/src/gtest in the line below.
add_subdirectory(/Users/datnguyen/googletest
                ${PROJECT_BINARY_DIR}/googletest)
include(CTest) # Include testing cmake package.
set(TEST_BINARY ${PROJECT_NAME}_test)

##################### Create your tests below this line ########################
add_executable(test_image test_image.cpp)
target_link_libraries(test_image
                      image
                      gtest gtest_main)
