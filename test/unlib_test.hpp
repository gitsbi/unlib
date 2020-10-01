#ifndef UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
#define UNLIB_TEST_UNLIB_UNIT_TESTS_HPP

#include <string>
#include <typeinfo>

#include <doctest/doctest.h>

namespace test {

std::string demangle(const char* mangled_symbol);
std::string demangle(const std::type_info& type_info);

template<typename T> std::string demangle()                   {return demangle(typeid(T).name());}
template<typename T> std::string demangle(const T& o)         {return demangle(typeid(o).name());}

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
