cmake_minimum_required(VERSION 3.15)
project(unlib)

include_directories(..)
find_package(Catch2 REQUIRED)

add_executable(unlib_test
        unit.hpp
        quantity.hpp
        test/unlib_test.cpp
        test/unlib_test.hpp
        test/test_unit.cpp
        test/test_quantity.cpp)

target_link_libraries(unlib_test PRIVATE Catch2::Catch2)
target_compile_features(unlib_test PRIVATE cxx_std_14)
