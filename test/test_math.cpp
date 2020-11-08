#include <unlib/math.hpp>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

TEST_CASE("math") {
	using namespace unlib;
	using scale_type = scale_t<1,2>;
	using value_type = double;
	using unit_a = unit<exponent_t<1>
	                   ,exponent_t<2>
	                   ,exponent_t<3>
	                   ,exponent_t<4>
	                   ,exponent_t<5>
	                   ,exponent_t<6>
	                   ,exponent_t<7>>;
	using quantity_a = quantity<unit_a, scale_type, value_type>;

	SUBCASE("sqrt") {
		using unit_sqrt = unit<exponent_t<1,2>
		                      ,exponent_t<2,2>
		                      ,exponent_t<3,2>
		                      ,exponent_t<4,2>
		                      ,exponent_t<5,2>
		                      ,exponent_t<6,2>
		                      ,exponent_t<7,2>>;
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
	SUBCASE("pow") {
		quantity_a a{2};

		const auto pow0 = unlib::pow<0>(a);
		CHECK(unlib::is_unit_dimensionless_v<decltype(pow0)::unit_type>);
		CHECK(pow0.get() == 1);

		const quantity_a pow1 = unlib::pow<1>(a);
		CHECK(pow1.get() == a.get());

		using unit_pow2 = unit<exponent_t<1*2>
		                      ,exponent_t<2*2>
		                      ,exponent_t<3*2>
		                      ,exponent_t<4*2>
		                      ,exponent_t<5*2>
		                      ,exponent_t<6*2>
		                      ,exponent_t<7*2>>;
		const auto pow2 = unlib::pow<2>(a);
		CHECK(typeid(decltype(pow2)::unit_type) == typeid(unit_pow2));
		CHECK(pow2.get() == std::pow(a.get(),2));

		const auto pow_neg2 = unlib::pow<-2>(a);
		using unit_pow_neg2 = unlib::reciprocal_unit_t<decltype(pow2)::unit_type>;
		CHECK(typeid(unit_pow_neg2) == typeid(decltype(pow_neg2)::unit_type));
		CHECK(pow_neg2.get() == std::pow(a.get(),-2));
	}

}
