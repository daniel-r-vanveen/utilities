#include_directories(include)

file(GLOB SOURCES "src/*.cxx")
add_library(Utilities SHARED ${SOURCES})
target_include_directories(Utilities PUBLIC include)
