find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(GLOB SOURCES "src/*.cxx")

# Create test from each source file: test_name.cxx -> test_name
foreach(_test_file ${SOURCES})
    get_filename_component(_test_name ${_test_file} NAME_WE)
    add_executable(${_test_name} ${_test_file})
    target_link_libraries(${_test_name} Utilities gtest gtest_main pthread)
    add_test(${_test_name} ${_test_name})
endforeach()
