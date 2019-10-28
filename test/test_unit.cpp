#include <unlib/unit.hpp>
#include <string>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch.hpp>

#include <unlib/test/unlib_test.hpp>

CATCH_TEST_CASE("exponents", "[unit]") {
	using namespace unlib;
	using unit_a = unit<exponent<1>
	                   ,exponent<2>
	                   ,exponent<3>
	                   ,exponent<4>
	                   ,exponent<5>
	                   ,exponent<6>
	                   ,exponent<7> >;

	CATCH_SECTION("exponents propagate to unit") {
		CATCH_CHECK( typeid(exponent<1>) == typeid(unit_a::            time_exponent) );
		CATCH_CHECK( typeid(exponent<2>) == typeid(unit_a::            mass_exponent) );
		CATCH_CHECK( typeid(exponent<3>) == typeid(unit_a::          length_exponent) );
		CATCH_CHECK( typeid(exponent<4>) == typeid(unit_a::         current_exponent) );
		CATCH_CHECK( typeid(exponent<5>) == typeid(unit_a::      luminosity_exponent) );
		CATCH_CHECK( typeid(exponent<6>) == typeid(unit_a::     temperature_exponent) );
		CATCH_CHECK( typeid(exponent<7>) == typeid(unit_a::substance_amount_exponent) );
	}

	CATCH_SECTION("get_exponent gets correct exponent") {
		CATCH_CHECK( typeid(exponent<1>) == typeid(detail::get_exponent<unit_a,unlib::detail::            time>::type) );
		CATCH_CHECK( typeid(exponent<2>) == typeid(detail::get_exponent<unit_a,unlib::detail::            mass>::type) );
		CATCH_CHECK( typeid(exponent<3>) == typeid(detail::get_exponent<unit_a,unlib::detail::          length>::type) );
		CATCH_CHECK( typeid(exponent<4>) == typeid(detail::get_exponent<unit_a,unlib::detail::         current>::type) );
		CATCH_CHECK( typeid(exponent<5>) == typeid(detail::get_exponent<unit_a,unlib::detail::      luminosity>::type) );
		CATCH_CHECK( typeid(exponent<6>) == typeid(detail::get_exponent<unit_a,unlib::detail::     temperature>::type) );
		CATCH_CHECK( typeid(exponent<7>) == typeid(detail::get_exponent<unit_a,unlib::detail::substance_amount>::type) );
	}

	using unit_list = detail::unsorted_unit_list_from_unit<unit_a>::type;

	CATCH_SECTION("unsorted_unit_list_from_unit creates unsorted list of all basic units") {
		using list = detail::unsorted_unit_list<pow_unit_t<unlib::detail::            time,1>
		                                       ,pow_unit_t<unlib::detail::            mass,2>
		                                       ,pow_unit_t<unlib::detail::          length,3>
		                                       ,pow_unit_t<unlib::detail::         current,4>
		                                       ,pow_unit_t<unlib::detail::      luminosity,5>
		                                       ,pow_unit_t<unlib::detail::     temperature,6>
		                                       ,pow_unit_t<unlib::detail::substance_amount,7> >;

		CATCH_REQUIRE( typeid(unit_list) == typeid(list) );
	}

	CATCH_SECTION("find_first_exponent finds correct exponents") {
		CATCH_CHECK( typeid(exponent<1>) == typeid(detail::find_first_exponent<unlib::detail::            time, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<2>) == typeid(detail::find_first_exponent<unlib::detail::            mass, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<3>) == typeid(detail::find_first_exponent<unlib::detail::          length, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<4>) == typeid(detail::find_first_exponent<unlib::detail::         current, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<5>) == typeid(detail::find_first_exponent<unlib::detail::      luminosity, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<6>) == typeid(detail::find_first_exponent<unlib::detail::     temperature, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<7>) == typeid(detail::find_first_exponent<unlib::detail::substance_amount, unit_list>::type) );

		CATCH_CHECK( typeid(exponent<2>) != typeid(detail::find_first_exponent<unlib::detail::            time, unit_list>::type) );
		CATCH_CHECK( typeid(exponent<1>) != typeid(detail::find_first_exponent<unlib::detail::            mass, unit_list>::type) );
	}
}

CATCH_TEST_CASE("unit type manipulations", "[unit]") {
	using namespace unlib;

	CATCH_SECTION("power") {
		using             time_0 = pow_unit_t<unlib::detail::            time,0>;
		using             mass_1 = pow_unit_t<unlib::detail::            mass,1>;
		using           length_2 = pow_unit_t<unlib::detail::          length,2>;
		using          current_3 = pow_unit_t<unlib::detail::         current,3>;
		using       luminosity_4 = pow_unit_t<unlib::detail::      luminosity,4>;
		using      temperature_5 = pow_unit_t<unlib::detail::     temperature,5>;
		using substance_amount_6 = pow_unit_t<unlib::detail::substance_amount,6>;
		CATCH_CHECK( typeid(            time_0) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>>) );
		CATCH_CHECK( typeid(            mass_1) == typeid(unit<exponent<0>,exponent<1>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>>) );
		CATCH_CHECK( typeid(          length_2) == typeid(unit<exponent<0>,exponent<0>,exponent<2>,exponent<0>,exponent<0>,exponent<0>,exponent<0>>) );
		CATCH_CHECK( typeid(         current_3) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<3>,exponent<0>,exponent<0>,exponent<0>>) );
		CATCH_CHECK( typeid(      luminosity_4) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<4>,exponent<0>,exponent<0>>) );
		CATCH_CHECK( typeid(     temperature_5) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<5>,exponent<0>>) );
		CATCH_CHECK( typeid(substance_amount_6) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<6>>) );

		using test_unit = unit< exponent<1>
		                      , exponent<2>
		                      , exponent<3>
		                      , exponent<4>
		                      , exponent<5>
		                      , exponent<6>
		                      , exponent<7> >;
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
		CHECK( typeid(pow_unit_t<test_unit, 3>) == typeid(cube_unit) );
		CHECK( typeid(pow_unit_t<test_unit, 2>) == typeid(squr_unit) );
		CHECK( typeid(pow_unit_t<test_unit, 1>) == typeid(test_unit) );
		CHECK( typeid(pow_unit_t<test_unit, 0>) == typeid(create_unit_t<>) );
		CHECK( typeid(pow_unit_t<test_unit,-1>) == typeid(reciprocal_unit_t<test_unit>) );
		CHECK( typeid(pow_unit_t<test_unit,-2>) == typeid(reciprocal_unit_t<squr_unit>) );
		CHECK( typeid(pow_unit_t<test_unit,-3>) == typeid(reciprocal_unit_t<cube_unit>) );
	}

	CATCH_SECTION("square") {
		using unit_b = unit< exponent<1>
		                   , exponent<2>
		                   , exponent<3>
		                   , exponent<4>
		                   , exponent<5>
		                   , exponent<6>
		                   , exponent<7> >;
		CATCH_CHECK( typeid(square_unit_t<unit_b>) == typeid(unit<exponent<2>,exponent<4>,exponent<6>,exponent<8>,exponent<10>,exponent<12>,exponent<14>>) );
	}

	CATCH_SECTION("cube") {
		using unit_b = unit< exponent<1>
		                   , exponent<2>
		                   , exponent<3>
		                   , exponent<4>
		                   , exponent<5>
		                   , exponent<6>
		                   , exponent<7> >;
		CATCH_CHECK( typeid(cube_unit_t<unit_b>) == typeid(unit<exponent<3>,exponent<6>,exponent<9>,exponent<12>,exponent<15>,exponent<18>,exponent<21>>) );
	}

	CATCH_SECTION("reciprocal") {
		using unit_b = unit< exponent<1>
		                   , exponent<2>
		                   , exponent<3>
		                   , exponent<4>
		                   , exponent<5>
		                   , exponent<6>
		                   , exponent<7> >;
		CATCH_CHECK( typeid(reciprocal_unit_t<unit_b>) == typeid(unit<exponent<-1>,exponent<-2>,exponent<-3>,exponent<-4>,exponent<-5>,exponent<-6>,exponent<-7>>) );
	}

	CATCH_SECTION("create_unit_t") {
		using unit_a = create_unit_t<unlib::detail::            time
		                            ,unlib::detail::            mass
		                            ,unlib::detail::          length
		                            ,unlib::detail::         current
		                            ,unlib::detail::      luminosity
		                            ,unlib::detail::     temperature
		                            ,unlib::detail::substance_amount >;

		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::            time_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::            mass_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::          length_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::         current_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::      luminosity_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::     temperature_exponent) );
		CATCH_CHECK( typeid(exponent<1,1>) == typeid(unit_a::substance_amount_exponent) );

		using unit_b = create_unit_t<pow_unit_t<unlib::detail::            time,1>
		                            ,pow_unit_t<unlib::detail::            mass,2>
		                            ,pow_unit_t<unlib::detail::          length,3>
		                            ,pow_unit_t<unlib::detail::         current,4>
		                            ,pow_unit_t<unlib::detail::      luminosity,5>
		                            ,pow_unit_t<unlib::detail::     temperature,6>
		                            ,pow_unit_t<unlib::detail::substance_amount,7> >;

		CATCH_CHECK( typeid(exponent<1>) == typeid(unit_b::            time_exponent) );
		CATCH_CHECK( typeid(exponent<2>) == typeid(unit_b::            mass_exponent) );
		CATCH_CHECK( typeid(exponent<3>) == typeid(unit_b::          length_exponent) );
		CATCH_CHECK( typeid(exponent<4>) == typeid(unit_b::         current_exponent) );
		CATCH_CHECK( typeid(exponent<5>) == typeid(unit_b::      luminosity_exponent) );
		CATCH_CHECK( typeid(exponent<6>) == typeid(unit_b::     temperature_exponent) );
		CATCH_CHECK( typeid(exponent<7>) == typeid(unit_b::substance_amount_exponent) );

		using unit_c = create_unit_t<unlib::detail::            mass
		                            ,unlib::detail::      luminosity
		                            ,unlib::detail::            time
		                            ,unlib::detail::          length
		                            ,unlib::detail::substance_amount
		                            ,unlib::detail::     temperature
		                            ,unlib::detail::         current >;

		CATCH_CHECK( typeid(unit_a) == typeid(unit_c) );
	}

	using unit_a = create_unit_t<pow_unit_t<unlib::detail::            time,1>
	                            ,pow_unit_t<unlib::detail::            mass,2>
	                            ,pow_unit_t<unlib::detail::          length,3>
	                            ,pow_unit_t<unlib::detail::         current,4>
	                            ,pow_unit_t<unlib::detail::      luminosity,5>
	                            ,pow_unit_t<unlib::detail::     temperature,6>
	                            ,pow_unit_t<unlib::detail::substance_amount,7> >;

	using unit_b = create_unit_t<pow_unit_t<unlib::detail::            time,7>
	                            ,pow_unit_t<unlib::detail::            mass,6>
	                            ,pow_unit_t<unlib::detail::          length,5>
	                            ,pow_unit_t<unlib::detail::         current,4>
	                            ,pow_unit_t<unlib::detail::      luminosity,3>
	                            ,pow_unit_t<unlib::detail::     temperature,2>
	                            ,pow_unit_t<unlib::detail::substance_amount,1> >;

	CATCH_SECTION("multiplication") {
		using unit_mul = create_unit_t<pow_unit_t<unlib::detail::            time,8>
		                              ,pow_unit_t<unlib::detail::            mass,8>
		                              ,pow_unit_t<unlib::detail::          length,8>
		                              ,pow_unit_t<unlib::detail::         current,8>
		                              ,pow_unit_t<unlib::detail::      luminosity,8>
		                              ,pow_unit_t<unlib::detail::     temperature,8>
		                              ,pow_unit_t<unlib::detail::substance_amount,8> >;
		using mul_unit = mul_unit_t<unit_a,unit_b>;
		CATCH_CHECK( typeid(mul_unit) == typeid(unit_mul) );
	}

	CATCH_SECTION("division") {
		using unit_div = create_unit_t<reciprocal_unit_t<pow_unit_t<unlib::detail::            time,6>>
		                              ,reciprocal_unit_t<pow_unit_t<unlib::detail::            mass,4>>
		                              ,reciprocal_unit_t<pow_unit_t<unlib::detail::          length,2>>
		                              ,                  pow_unit_t<unlib::detail::         current,0>
		                              ,                  pow_unit_t<unlib::detail::      luminosity,2>
		                              ,                  pow_unit_t<unlib::detail::     temperature,4>
		                              ,                  pow_unit_t<unlib::detail::substance_amount,6> >;

		CATCH_CHECK( typeid(div_unit_t<unit_a,unit_b>) == typeid(unit_div) );
	}
}

CATCH_TEST_CASE("unit compatibility", "[unit]") {
	using namespace unlib;
	using unit_a = create_unit_t<pow_unit_t<unlib::detail::            time,1>
	                            ,pow_unit_t<unlib::detail::            mass,2>
	                            ,pow_unit_t<unlib::detail::          length,3>
	                            ,pow_unit_t<unlib::detail::         current,4>
	                            ,pow_unit_t<unlib::detail::      luminosity,5>
	                            ,pow_unit_t<unlib::detail::     temperature,6>
	                            ,pow_unit_t<unlib::detail::substance_amount,7> >;

	CATCH_SECTION("compatibility") {
		using unit_b = create_unit_t<pow_unit_t<unlib::detail::            time,1>
		                            ,pow_unit_t<unlib::detail::            mass,2>
		                            ,pow_unit_t<unlib::detail::     temperature,6>
		                            ,pow_unit_t<unlib::detail::      luminosity,5>
		                            ,pow_unit_t<unlib::detail::          length,3>
		                            ,pow_unit_t<unlib::detail::substance_amount,7>
		                            ,pow_unit_t<unlib::detail::         current,4> >;
		CATCH_CHECK( are_units_compatible<unit_a, unit_b>::value );
	}
	CATCH_SECTION("incompatibility") {
		using unit_b = create_unit_t<pow_unit_t<unlib::detail::            time,0>
		                            ,pow_unit_t<unlib::detail::            mass,2>
		                            ,pow_unit_t<unlib::detail::          length,3>
		                            ,pow_unit_t<unlib::detail::         current,4>
		                            ,pow_unit_t<unlib::detail::      luminosity,5>
		                            ,pow_unit_t<unlib::detail::     temperature,6>
		                            ,pow_unit_t<unlib::detail::substance_amount,7> >;
		CATCH_CHECK( not are_units_compatible<unit_a, unit_b>::value );
	}
}

CATCH_TEST_CASE("dimensionlessness", "[unit]") {
	using namespace unlib;
	CATCH_SECTION("dimensionless") {
		CATCH_CHECK( typeid(dimensionless) == typeid(unit<exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>,exponent<0>>) );
	}
	CATCH_SECTION("is_dimensionless") {
		using unit_b = create_unit_t<>;
		using unit = create_unit_t<pow_unit_t<unlib::detail::            time,1>
		                          ,pow_unit_t<unlib::detail::            mass,2>
		                          ,pow_unit_t<unlib::detail::          length,3>
		                          ,pow_unit_t<unlib::detail::         current,4>
		                          ,pow_unit_t<unlib::detail::      luminosity,5>
		                          ,pow_unit_t<unlib::detail::     temperature,6>
		                          ,pow_unit_t<unlib::detail::substance_amount,7> >;

		CATCH_CHECK( unit_is_dimensionless<unit_b>::value );
		CATCH_CHECK( not unit_is_dimensionless<unit>::value );
	}
}
