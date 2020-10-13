cmake_minimum_required(VERSION 3.15)
project(unlib)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message("detected VC")
  add_compile_options("/permissive-" "/std:c++14")
endif()

include_directories(..)

include_directories(doctest)

add_executable(unlib_test
        unit.hpp
        quantity.hpp
        common.hpp
        math.hpp
        test/unlib_test.hpp
        test/unlib_test.cpp
        test/test_unit.cpp
        test/test_quantity.cpp
        test/test_common.cpp
        test/test_math.cpp)

target_compile_features(unlib_test PRIVATE cxx_std_14)
