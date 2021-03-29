#include <unlib/unlib/dimension.hpp>
#include <string>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

TEST_CASE("dimensions") {

	SUBCASE("exponents") {
	using namespace unlib;
	using dimension_a = dimension<exponent_t<1>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7> >;

	SUBCASE("exponents propagate to dimension") {
		CHECK( typeid(exponent_t<1>) == typeid(dimension_a::              time_exponent) );
		CHECK( typeid(exponent_t<2>) == typeid(dimension_a::              mass_exponent) );
		CHECK( typeid(exponent_t<3>) == typeid(dimension_a::            length_exponent) );
		CHECK( typeid(exponent_t<4>) == typeid(dimension_a::           current_exponent) );
		CHECK( typeid(exponent_t<5>) == typeid(dimension_a::       temperature_exponent) );
		CHECK( typeid(exponent_t<6>) == typeid(dimension_a::  substance_amount_exponent) );
		CHECK( typeid(exponent_t<7>) == typeid(dimension_a::luminous_intensity_exponent) );
	}
}

SUBCASE("dimension type manipulations") {
	using namespace unlib;

	SUBCASE("power") {
		using dimension_1 = pow_dimension_t<unlib::              time,std::ratio<1>>;
		using dimension_2 = pow_dimension_t<unlib::              mass,std::ratio<2>>;
		using dimension_3 = pow_dimension_t<unlib::            length,std::ratio<3>>;
		using dimension_4 = pow_dimension_t<unlib::           current,std::ratio<4>>;
		using dimension_5 = pow_dimension_t<unlib::       temperature,std::ratio<5>>;
		using dimension_6 = pow_dimension_t<unlib::  substance_amount,std::ratio<6>>;
		using dimension_7 = pow_dimension_t<unlib::luminous_intensity,std::ratio<7>>;
		CHECK( typeid(dimension_1) == typeid(dimension<exponent_t<1>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(dimension_2) == typeid(dimension<exponent_t<0>,exponent_t<2>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(dimension_3) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<3>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(dimension_4) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<4>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(dimension_5) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<5>,exponent_t<0>,exponent_t<0>>) );
		CHECK( typeid(dimension_6) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<6>,exponent_t<0>>) );
		CHECK( typeid(dimension_7) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<7>>) );

		using test_dimension = dimension< exponent_t<1>
		                                , exponent_t<2>
		                                , exponent_t<3>
		                                , exponent_t<4>
		                                , exponent_t<5>
		                                , exponent_t<6>
		                                , exponent_t<7> >;
		using squr_dimension = dimension< std::ratio_add<test_dimension::              time_exponent,test_dimension::              time_exponent>
		                                , std::ratio_add<test_dimension::              mass_exponent,test_dimension::              mass_exponent>
		                                , std::ratio_add<test_dimension::            length_exponent,test_dimension::            length_exponent>
		                                , std::ratio_add<test_dimension::           current_exponent,test_dimension::           current_exponent>
		                                , std::ratio_add<test_dimension::       temperature_exponent,test_dimension::       temperature_exponent>
		                                , std::ratio_add<test_dimension::  substance_amount_exponent,test_dimension::  substance_amount_exponent>
		                                , std::ratio_add<test_dimension::luminous_intensity_exponent,test_dimension::luminous_intensity_exponent> >;
		using cube_dimension = dimension< std::ratio_add<squr_dimension::              time_exponent,test_dimension::              time_exponent>
		                                , std::ratio_add<squr_dimension::              mass_exponent,test_dimension::              mass_exponent>
		                                , std::ratio_add<squr_dimension::            length_exponent,test_dimension::            length_exponent>
		                                , std::ratio_add<squr_dimension::           current_exponent,test_dimension::           current_exponent>
		                                , std::ratio_add<squr_dimension::       temperature_exponent,test_dimension::       temperature_exponent>
		                                , std::ratio_add<squr_dimension::  substance_amount_exponent,test_dimension::  substance_amount_exponent>
		                                , std::ratio_add<squr_dimension::luminous_intensity_exponent,test_dimension::luminous_intensity_exponent> >;
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio< 3>>) == typeid(                       cube_dimension    ) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio< 2>>) == typeid(                       squr_dimension    ) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio< 1>>) == typeid(                       test_dimension    ) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio< 0>>) == typeid(                            dimensionless) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio<-1>>) == typeid(reciprocal_dimension_t<test_dimension>   ) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio<-2>>) == typeid(reciprocal_dimension_t<squr_dimension>   ) );
		CHECK( typeid(pow_dimension_t<test_dimension,std::ratio<-3>>) == typeid(reciprocal_dimension_t<cube_dimension>   ) );
	}

	SUBCASE("square") {
		using dimension_b = dimension< exponent_t<1>
		                             , exponent_t<2>
		                             , exponent_t<3>
		                             , exponent_t<4>
		                             , exponent_t<5>
		                             , exponent_t<6>
		                             , exponent_t<7> >;
		CHECK( typeid(square_dimension_t<dimension_b>) == typeid(dimension<exponent_t<2>,exponent_t<4>,exponent_t<6>,exponent_t<8>,exponent_t<10>,exponent_t<12>,exponent_t<14>>) );
	}

	SUBCASE("cube") {
		using dimension_b = dimension< exponent_t<1>
		                             , exponent_t<2>
		                             , exponent_t<3>
		                             , exponent_t<4>
		                             , exponent_t<5>
		                             , exponent_t<6>
		                             , exponent_t<7> >;
		CHECK( typeid(cube_dimension_t<dimension_b>) == typeid(dimension<exponent_t<3>,exponent_t<6>,exponent_t<9>,exponent_t<12>,exponent_t<15>,exponent_t<18>,exponent_t<21>>) );
	}

	SUBCASE("reciprocal") {
		using dimension_b = dimension< exponent_t<1>
		                             , exponent_t<2>
		                             , exponent_t<3>
		                             , exponent_t<4>
		                             , exponent_t<5>
		                             , exponent_t<6>
		                             , exponent_t<7> >;
		CHECK( typeid(reciprocal_dimension_t<dimension_b>) == typeid(dimension<exponent_t<-1>,exponent_t<-2>,exponent_t<-3>,exponent_t<-4>,exponent_t<-5>,exponent_t<-6>,exponent_t<-7>>) );
	}

	using dimension_a = dimension<exponent_t<1>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7> >;

	using dimension_b = dimension<exponent_t<7>
	                             ,exponent_t<6>
	                             ,exponent_t<5>
	                             ,exponent_t<4>
	                             ,exponent_t<3>
	                             ,exponent_t<2>
	                             ,exponent_t<1> >;

	SUBCASE("multiplication") {
		using dimension_mul = dimension<exponent_t<8>
		                               ,exponent_t<8>
		                               ,exponent_t<8>
		                               ,exponent_t<8>
		                               ,exponent_t<8>
		                               ,exponent_t<8>
		                               ,exponent_t<8> >;
		using mul_dimension = mul_dimension_t<dimension_a,dimension_b>;
		CHECK( typeid(mul_dimension) == typeid(dimension_mul) );
	}

	SUBCASE("division") {
		using dimension_div = dimension<exponent_t<-6>
		                               ,exponent_t<-4>
		                               ,exponent_t<-2>
		                               ,exponent_t< 0>
		                               ,exponent_t< 2>
		                               ,exponent_t< 4>
		                               ,exponent_t< 6> >;

		CHECK( typeid(div_dimension_t<dimension_a,dimension_b>) == typeid(dimension_div) );
	}
}

SUBCASE("dimension compatibility") {
	using namespace unlib;
	using dimension_a = dimension<exponent_t<1>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7> >;

	using dimension_b = dimension<exponent_t<1>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7> >;

	using dimension_c = dimension<exponent_t<0>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7> >;

	CHECK(    are_dimensions_compatible_v<dimension_a, dimension_b>);
	CHECK(not are_dimensions_compatible_v<dimension_a, dimension_c>);
	CHECK(not are_dimensions_compatible_v<dimension_a, dimensionless>);
	CHECK(not are_dimensions_compatible_v<dimension_b, dimensionless>);
	CHECK(not are_dimensions_compatible_v<dimension_c, dimensionless>);
}

SUBCASE("dimensionlessness") {
	using namespace unlib;
	CHECK( typeid(dimensionless) == typeid(dimension<exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>,exponent_t<0>>) );
}

}
