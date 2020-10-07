#ifndef UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
#define UNLIB_TEST_UNLIB_UNIT_TESTS_HPP

#include <string>
#include <typeinfo>

#include <doctest/doctest.h>

#include <unlib/unit.hpp>
#include <unlib/quantity.hpp>

namespace test {

std::string demangle(const char* mangled_symbol);
std::string demangle(const std::type_info& type_info);

template<typename T> std::string demangle()                               {return demangle(typeid(T).name());}
template<typename T> std::string demangle(const T& o)                     {return demangle(typeid(o).name());}

template<typename U1, typename U2>
constexpr bool is_same_unit()                                             {return unlib::are_units_compatible_v<U1,U2>;}

template< typename U1
        , typename U2
        , typename S
        , typename V
        , typename T>
constexpr bool is_same_unit(const unlib::quantity<U2,S,V,T>)              {return is_same_unit<U1, U2>();}

}

namespace doctest {

template<>
struct StringMaker<std::type_info> {
	static String convert(const std::type_info& mangled_symbol) {
		return test::demangle(mangled_symbol).c_str();
	}
};

}

#endif // UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
