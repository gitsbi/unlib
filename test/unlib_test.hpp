#ifndef UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
#define UNLIB_TEST_UNLIB_UNIT_TESTS_HPP

#include <string>
#include <typeinfo>

#include <doctest/doctest.h>

#include <unlib/unlib/dimension.hpp>
#include <unlib/quantity.hpp>

namespace test {

std::string demangle(const char* mangled_symbol);
std::string demangle(const std::type_info& type_info);

template<typename T> std::string demangle()                               {return demangle(typeid(T).name());}
template<typename T> std::string demangle(const T& o)                     {return demangle(typeid(o).name());}

template<typename D1, typename D2>
constexpr bool is_same_dimension()                                        {return unlib::are_dimensions_compatible_v<D1,D2>;}

template< typename D1
        , typename D2
        , typename S
        , typename V
        , typename T>
constexpr bool is_same_dimension(const unlib::quantity<D2,S,V,T>)         {return is_same_dimension<D1, D2>();}

}

namespace doctest {

template<>
struct StringMaker<std::type_info> {
	static String convert(const std::type_info& ti) {
		return test::demangle(ti).c_str();
	}
};

}

#endif // UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
