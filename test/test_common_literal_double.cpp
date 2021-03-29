#define UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE long double

#include <unlib/common.hpp>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

TEST_CASE("common quantities") {

SUBCASE("literal operator result types") {
	using namespace unlib::literals;
	const auto kW = 42._kW;
	using kW_t = decltype(kW);
	SUBCASE("long long") {
		CHECK( typeid(kW_t::value_type) == typeid(long double) );
	}
}

}
