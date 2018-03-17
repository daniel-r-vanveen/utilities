cmake_minimum_required(VERSION 3.5.0)
project (utilities)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(CTest)

add_subdirectory(libUtilities)
add_subdirectory(test)
