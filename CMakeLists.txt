cmake_minimum_required(VERSION 3.5.0)
project (utilities)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB SOURCES "src/*.cxx")

add_library(Utilities SHARED ${SOURCES})