cmake_minimum_required(VERSION 3.15)
project(unlib)

enable_testing()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  message("detected VC")
  add_compile_options("/permissive-" "/std:c++14")
endif()

include_directories(..)

include_directories(doctest)

add_executable(unlib_test
        unlib/ratio.hpp
        unlib/dimension.hpp
        unlib/tag.hpp
        unlib/scaling.hpp
        quantity.hpp
        common.hpp
        math.hpp
        limits.hpp
        test/unlib_test.hpp
        test/unlib_test.cpp
        test/test_ratio.cpp
        test/test_dimension.cpp
        test/test_quantity.cpp
        test/test_limits.cpp
        test/test_common.cpp
        test/test_common_literal_int.cpp
        test/test_common_literal_long.cpp
        test/test_common_literal_float.cpp
        test/test_common_literal_double.cpp
        test/test_math.cpp
)

target_compile_features(unlib_test PRIVATE cxx_std_14)

add_test(NAME unlib_unit_tests COMMAND unlib_test)
