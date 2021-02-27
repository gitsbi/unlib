#include <unlib/unit.hpp>
#include <string>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

namespace {

using e0 = unlib::exponent_t<0>;

template<typename Unit>
struct unsorted_unit_list_from_unit;
template<typename E1, typename E2, typename E3, typename E4, typename E5, typename E6, typename E7>
struct unsorted_unit_list_from_unit<unlib::unit<E1,E2,E3,E4,E5,E6,E7>> {
	using type = unlib::detail::unsorted_unit_list< unlib::unit< E1, e0, e0, e0, e0, e0, e0 >
	                                              , unlib::unit< e0, E2, e0, e0, e0, e0, e0 >
	                                              , unlib::unit< e0, e0, E3, e0, e0, e0, e0 >
	                                              , unlib::unit< e0, e0, e0, E4, e0, e0, e0 >
	                                              , unlib::unit< e0, e0, e0, e0, E5, e0, e0 >
	                                              , unlib::unit< e0, e0, e0, e0, e0, E6, e0 >
	                                              , unlib::unit< e0, e0, e0, e0, e0, e0, E7 > >;
};

}
TEST_CASE("units") {

	SUBCASE("exponents") {
	using namespace unlib;
	using unit_a = unit<exponent_t<1>
	                   ,exponent_t<2>
	                   ,exponent_t<3>
	                   ,exponent_t<4>
	                   ,exponent_t<5>
	                   ,exponent_t<6>
	                   ,exponent_t<7> >;

	SUBCASE("exponents propagate to unit") {
		CHECK( typeid(exponent_t<1>) == typeid(unit_a::            time_exponent) );
		CHECK( typeid(exponent_t<2>) == typeid(unit_a::            mass_exponent) );
		CHECK( typeid(exponent_t<3>) == typeid(unit_a::          length_exponent) );
		CHECK( typeid(exponent_t<4>) == typeid(unit_a::         current_exponent) );
		CHECK( typeid(exponent_t<5>) == typeid(unit_a::      luminosity_exponent) );
		CHECK( typeid(exponent_t<6>) == typeid(unit_a::     temperature_exponent) );
		CHECK( typeid(exponent_t<7>) == typeid(unit_a::substance_amount_exponent) );
	}

	SUBCASE("get_exponent gets correct exponent") {
		CHECK( typeid(exponent_t<1>) == typeid(detail::get_exponent_t<unit_a,unlib::            time>) );
		CHECK( typeid(exponent_t<2>) == typeid(detail::get_exponent_t<unit_a,unlib::            mass>) );
		CHECK( typeid(exponent_t<3>) == typeid(detail::get_exponent_t<unit_a,unlib::          length>) );
		CHECK( typeid(exponent_t<4>) == typeid(detail::get_exponent_t<unit_a,unlib::         current>) );
		CHECK( typeid(exponent_t<5>) == typeid(detail::get_exponent_t<unit_a,unlib::      luminosity>) );
		CHECK( typeid(exponent_t<6>) == typeid(detail::get_exponent_t<unit_a,unlib::     temperature>) );
		CHECK( typeid(exponent_t<7>) == typeid(detail::get_exponent_t<unit_a,unlib::substance_amount>) );
	}

	SUBCASE("find_first_exponent finds correct exponents") {
		using unit_list = unsorted_unit_list_from_unit<unit_a>::type;
		CHECK( typeid(exponent_t<1>) == typeid(detail::find_first_exponent_t<unlib::            time, unit_list>) );
		CHECK( typeid(exponent_t<2>) == typeid(detail::find_first_exponent_t<unlib::            mass, unit_list>) );
		CHECK( typeid(exponent_t<3>) == typeid(detail::find_first_exponent_t<unlib::          length, unit_list>) );
		CHECK( typeid(exponent_t<4>) == typeid(detail::find_first_exponent_t<unlib::         current, unit_list>) );
		CHECK( typeid(exponent_t<5>) == typeid(detail::find_first_exponent_t<unlib::      luminosity, unit_list>) );
		CHECK( typeid(exponent_t<6>) == typeid(detail::find_first_exponent_t<unlib::     temperature, unit_list>) );
		CHECK( typeid(exponent_t<7>) == typeid(detail::find_first_exponent_t<unlib::substance_amount, unit_list>) );

		CHECK( typeid(exponent_t<2>) != typeid(detail::find_first_exponent_t<unlib::            time, unit_list>) );
		CHECK( typeid(exponent_t<1>) != typeid(detail::find_first_exponent_t<unlib::            mass, unit_list>) );
	}
}

SUBCASE("unit type manipulations") {
	using namespace unlib;

	SUBCASE("power") {
		using unit_1 = pow_unit_t<unlib::            time,std::ratio<1>>;
		using unit_2 = pow_unit_t<unlib::            mass,std::ratio<2>>;
		using unit_3 = pow_unit_t<unlib::          length,std::ratio<3>>;
		using unit_4 = pow_unit_t<unlib::         current,std::ratio<4>>;
		using unit_5 = pow_unit_t<unlib::      luminosity,std::ratio<5>>;
		using unit_6 = pow_unit_t<unlib::     temperature,std::ratio<6>>;
		using unit_7 = pow_unit_t<unlib::substance_amount,std::ratio<7>>;
		CHECK( typeid(unit_1) == typeid(unit<exponent_t<1>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(unit_2) == typeid(unit<exponent_t<0>,exponent_t<2>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(unit_3) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<3>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(unit_4) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<4>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(unit_5) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<5>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(unit_6) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<6>,exponent_t<0>>) );
		CHECK( typeid(unit_7) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<7>>) );

		using test_unit = unit< exponent_t<1>
		                      , exponent_t<2>
		                      , exponent_t<3>
		                      , exponent_t<4>
		                      , exponent_t<5>
		                      , exponent_t<6>
		                      , exponent_t<7> >;
		using squr_unit = unit< std::ratio_add<test_unit::            time_exponent,test_unit::            time_exponent>
		                      , std::ratio_add<test_unit::            mass_exponent,test_unit::            mass_exponent>
		                      , std::ratio_add<test_unit::          length_exponent,test_unit::          length_exponent>
		                      , std::ratio_add<test_unit::         current_exponent,test_unit::         current_exponent>
		                      , std::ratio_add<test_unit::      luminosity_exponent,test_unit::      luminosity_exponent>
		                      , std::ratio_add<test_unit::     temperature_exponent,test_unit::     temperature_exponent>
		                      , std::ratio_add<test_unit::substance_amount_exponent,test_unit::substance_amount_exponent> >;
		using cube_unit = unit< std::ratio_add<squr_unit::            time_exponent,test_unit::            time_exponent>
		                      , std::ratio_add<squr_unit::            mass_exponent,test_unit::            mass_exponent>
		                      , std::ratio_add<squr_unit::          length_exponent,test_unit::          length_exponent>
		                      , std::ratio_add<squr_unit::         current_exponent,test_unit::         current_exponent>
		                      , std::ratio_add<squr_unit::      luminosity_exponent,test_unit::      luminosity_exponent>
		                      , std::ratio_add<squr_unit::     temperature_exponent,test_unit::     temperature_exponent>
		                      , std::ratio_add<squr_unit::substance_amount_exponent,test_unit::substance_amount_exponent> >;
		CHECK( typeid(pow_unit_t<test_unit,std::ratio< 3>>) == typeid(                  cube_unit ) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio< 2>>) == typeid(                  squr_unit ) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio< 1>>) == typeid(                  test_unit ) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio< 0>>) == typeid(                  unit_t<>  ) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio<-1>>) == typeid(reciprocal_unit_t<test_unit>) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio<-2>>) == typeid(reciprocal_unit_t<squr_unit>) );
		CHECK( typeid(pow_unit_t<test_unit,std::ratio<-3>>) == typeid(reciprocal_unit_t<cube_unit>) );
	}

	SUBCASE("square") {
		using unit_b = unit< exponent_t<1>
		                   , exponent_t<2>
		                   , exponent_t<3>
		                   , exponent_t<4>
		                   , exponent_t<5>
		                   , exponent_t<6>
		                   , exponent_t<7> >;
		CHECK( typeid(square_unit_t<unit_b>) == typeid(unit<exponent_t<2>,exponent_t<4>,exponent_t<6>,exponent_t<8>,exponent_t<10>,exponent_t<12>,exponent_t<14>>) );
	}

	SUBCASE("cube") {
		using unit_b = unit< exponent_t<1>
		                   , exponent_t<2>
		                   , exponent_t<3>
		                   , exponent_t<4>
		                   , exponent_t<5>
		                   , exponent_t<6>
		                   , exponent_t<7> >;
		CHECK( typeid(cube_unit_t<unit_b>) == typeid(unit<exponent_t<3>,exponent_t<6>,exponent_t<9>,exponent_t<12>,exponent_t<15>,exponent_t<18>,exponent_t<21>>) );
	}

	SUBCASE("reciprocal") {
		using unit_b = unit< exponent_t<1>
		                   , exponent_t<2>
		                   , exponent_t<3>
		                   , exponent_t<4>
		                   , exponent_t<5>
		                   , exponent_t<6>
		                   , exponent_t<7> >;
		CHECK( typeid(reciprocal_unit_t<unit_b>) == typeid(unit<exponent_t<-1>,exponent_t<-2>,exponent_t<-3>,exponent_t<-4>,exponent_t<-5>,exponent_t<-6>,exponent_t<-7>>) );
	}

	SUBCASE("unit_t") {
		using unit_a = unit_t<unlib::            time
		                     ,unlib::            mass
		                     ,unlib::          length
		                     ,unlib::         current
		                     ,unlib::      luminosity
		                     ,unlib::     temperature
		                     ,unlib::substance_amount >;

		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::            time_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::            mass_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::          length_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::         current_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::      luminosity_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::     temperature_exponent) );
		CHECK( typeid(exponent_t<1,1>) == typeid(unit_a::substance_amount_exponent) );

		using unit_b = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
		                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
		                     ,pow_unit_t<unlib::          length,std::ratio<3>>
		                     ,pow_unit_t<unlib::         current,std::ratio<4>>
		                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
		                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
		                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

		CHECK( typeid(exponent_t<1>) == typeid(unit_b::            time_exponent) );
		CHECK( typeid(exponent_t<2>) == typeid(unit_b::            mass_exponent) );
		CHECK( typeid(exponent_t<3>) == typeid(unit_b::          length_exponent) );
		CHECK( typeid(exponent_t<4>) == typeid(unit_b::         current_exponent) );
		CHECK( typeid(exponent_t<5>) == typeid(unit_b::      luminosity_exponent) );
		CHECK( typeid(exponent_t<6>) == typeid(unit_b::     temperature_exponent) );
		CHECK( typeid(exponent_t<7>) == typeid(unit_b::substance_amount_exponent) );
	}

	using unit_a = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
	                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
	                     ,pow_unit_t<unlib::          length,std::ratio<3>>
	                     ,pow_unit_t<unlib::         current,std::ratio<4>>
	                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
	                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
	                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

	using unit_b = unit_t<pow_unit_t<unlib::            time,std::ratio<7>>
	                     ,pow_unit_t<unlib::            mass,std::ratio<6>>
	                     ,pow_unit_t<unlib::          length,std::ratio<5>>
	                     ,pow_unit_t<unlib::         current,std::ratio<4>>
	                     ,pow_unit_t<unlib::      luminosity,std::ratio<3>>
	                     ,pow_unit_t<unlib::     temperature,std::ratio<2>>
	                     ,pow_unit_t<unlib::substance_amount,std::ratio<1>> >;

	SUBCASE("multiplication") {
		using unit_mul = unit_t<pow_unit_t<unlib::            time,std::ratio<8>>
		                       ,pow_unit_t<unlib::            mass,std::ratio<8>>
		                       ,pow_unit_t<unlib::          length,std::ratio<8>>
		                       ,pow_unit_t<unlib::         current,std::ratio<8>>
		                       ,pow_unit_t<unlib::      luminosity,std::ratio<8>>
		                       ,pow_unit_t<unlib::     temperature,std::ratio<8>>
		                       ,pow_unit_t<unlib::substance_amount,std::ratio<8>> >;
		using mul_unit = mul_unit_t<unit_a,unit_b>;
		CHECK( typeid(mul_unit) == typeid(unit_mul) );
	}

	SUBCASE("division") {
		using unit_div = unit_t<reciprocal_unit_t<pow_unit_t<unlib::            time,std::ratio<6>>>
		                       ,reciprocal_unit_t<pow_unit_t<unlib::            mass,std::ratio<4>>>
		                       ,reciprocal_unit_t<pow_unit_t<unlib::          length,std::ratio<2>>>
		                       ,                  pow_unit_t<unlib::         current,std::ratio<0>>
		                       ,                  pow_unit_t<unlib::      luminosity,std::ratio<2>>
		                       ,                  pow_unit_t<unlib::     temperature,std::ratio<4>>
		                       ,                  pow_unit_t<unlib::substance_amount,std::ratio<6>> >;

		CHECK( typeid(div_unit_t<unit_a,unit_b>) == typeid(unit_div) );
	}
}

SUBCASE("unit compatibility") {
	using namespace unlib;
	using unit_a = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
	                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
	                     ,pow_unit_t<unlib::          length,std::ratio<3>>
	                     ,pow_unit_t<unlib::         current,std::ratio<4>>
	                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
	                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
	                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

	using unit_b = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
	                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
	                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
	                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
	                     ,pow_unit_t<unlib::          length,std::ratio<3>>
	                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>>
	                     ,pow_unit_t<unlib::         current,std::ratio<4>> >;

	using unit_c = unit_t<pow_unit_t<unlib::            time,std::ratio<0>>
	                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
	                     ,pow_unit_t<unlib::          length,std::ratio<3>>
	                     ,pow_unit_t<unlib::         current,std::ratio<4>>
	                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
	                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
	                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

	CHECK(    are_units_compatible_v<unit_a, unit_b>);
	CHECK(not are_units_compatible_v<unit_a, unit_c>);
	CHECK(not are_units_compatible_v<unit_a, dimensionless>);
	CHECK(not are_units_compatible_v<unit_b, dimensionless>);
	CHECK(not are_units_compatible_v<unit_c, dimensionless>);
}

SUBCASE("dimensionlessness") {
	using namespace unlib;
	SUBCASE("dimensionless") {
		CHECK( typeid(dimensionless) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
	}
	using unit_a = unit_t<>;
	REQUIRE( typeid(unit_a) == typeid(unit<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
	SUBCASE("is_unit_dimensionless") {
		using unit_b = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
		                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
		                     ,pow_unit_t<unlib::          length,std::ratio<3>>
		                     ,pow_unit_t<unlib::         current,std::ratio<4>>
		                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
		                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
		                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

		CHECK(     is_unit_dimensionless_v<unit_a> );
		CHECK( not is_unit_dimensionless_v<unit_b> );
	}

	SUBCASE("type comparison") {
		using unit_c = unit_t<pow_unit_t<unlib::            time,std::ratio<1>>
		                     ,pow_unit_t<unlib::            mass,std::ratio<2>>
		                     ,pow_unit_t<unlib::          length,std::ratio<3>>
		                     ,pow_unit_t<unlib::         current,std::ratio<4>>
		                     ,pow_unit_t<unlib::      luminosity,std::ratio<5>>
		                     ,pow_unit_t<unlib::     temperature,std::ratio<6>>
		                     ,pow_unit_t<unlib::substance_amount,std::ratio<7>> >;
		CHECK(not are_units_compatible_v<unit_a , unit_c>);
	}
}

}
