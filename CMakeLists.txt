cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(CMAKE_INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/include)
set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/libs/lib)

enable_testing()

include(build_libraries)

find_path(GOOGLE_TEST_INCLUDE gtest/gtest.h)
find_library(LIB_GOOGLE_TEST gtest)

add_subdirectory(imbo5)
