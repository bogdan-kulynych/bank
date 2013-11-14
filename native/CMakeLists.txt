cmake_minimum_required (VERSION 2.8)

project (bank)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory (config)
add_subdirectory (src)
# add_subdirectory (test)

enable_testing ()
add_test (NAME unit_tests COMMAND run_unit_tests)
