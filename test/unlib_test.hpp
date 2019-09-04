#ifndef UNLIB_TEST_UNLIB_UNIT_TESTS_HPP
#define UNLIB_TEST_UNLIB_UNIT_TESTS_HPP

#include <string>
#include <typeinfo>

#include <catch2/catch.hpp>

namespace test {

std::string demangle(const char* mangled_symbol);
std::string demangle(const std::type_info& type_info);

template<typename T> std::string demangle()                   {return demangle(typeid(T).name());}
template<typename T> std::string demangle(const T& o)         {return demangle(typeid(o).name());}

}

namespace Catch {

template<>
struct StringMaker<std::type_info> {
	static std::string convert(const std::type_info& mangled_symbol) {
		return test::demangle(mangled_symbol);
	}
};

}

#endif // UNLIB_TEST_UNLIB_UNIT_TESTS_HPP