#include <unlib/math.hpp>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

TEST_CASE("math") {
	using namespace unlib;
	using scale_type = scale<1,2>;
	using value_type = double;
	using unit_a = unit<exponent<1>
	                   ,exponent<2>
	                   ,exponent<3>
	                   ,exponent<4>
	                   ,exponent<5>
	                   ,exponent<6>
	                   ,exponent<7>>;
	using quantity_a = quantity<unit_a, scale_type, value_type>;

	SUBCASE("sqrt") {
		using unit_sqrt = unit<exponent<1,2>
		                      ,exponent<2,2>
		                      ,exponent<3,2>
		                      ,exponent<4,2>
		                      ,exponent<5,2>
		                      ,exponent<6,2>
		                      ,exponent<7,2>>;
		CHECK(typeid(unlib::sqrt_unit_t<unit_a>) == typeid(unit_sqrt));

		using quantity_sqrt = quantity<unit_sqrt, scale_type, value_type>;
		CHECK(typeid(quantity_sqrt) == typeid(unlib::sqrt_quantity_t<quantity_a>));

		const auto sqroot = unlib::sqrt(quantity_a{81});
		CHECK(typeid(sqroot) == typeid(quantity_sqrt));

		CHECK(sqroot.get() == doctest::Approx(9));
	}

	SUBCASE("abs") {
		quantity_a gt{ 42};
		quantity_a z {  0};
		quantity_a lt{-gt.get()};

		CHECK(is_near(unlib::abs(gt),gt));
		CHECK(is_near(unlib::abs( z), z));
		CHECK(is_near(unlib::abs(lt),gt));
	}
}
