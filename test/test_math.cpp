#include <unlib/math.hpp>

#include <doctest/doctest.h>

TEST_CASE("math") {
	using namespace unlib;

	using value_type = double;
	using dimension_a = dimension<exponent_t<1>
	                             ,exponent_t<2>
	                             ,exponent_t<3>
	                             ,exponent_t<4>
	                             ,exponent_t<5>
	                             ,exponent_t<6>
	                             ,exponent_t<7>>;
	using scale_type = scale_t<100,1>;
	using quantity_a = quantity<dimension_a, scale_type, value_type>;

	SUBCASE("abs") {
		quantity_a gt{ 42};
		quantity_a z {  0};
		quantity_a lt{-gt.get()};

		CHECK(is_near(unlib::abs(gt),gt));
		CHECK(is_near(unlib::abs( z), z));
		CHECK(is_near(unlib::abs(lt),gt));
	}


	SUBCASE("pow") {
		using quantity0 = quantity<dimensionless, scale_t<1>, value_type>;
		using quantity1 = quantity<dimension_a, scale_t<1000000,1>, value_type>;
		using quantity2 = quantity< dimension< exponent_t<1+1>
		                                     , exponent_t<2+2>
		                                     , exponent_t<3+3>
		                                     , exponent_t<4+4>
		                                     , exponent_t<5+5>
		                                     , exponent_t<6+6>
		                                     , exponent_t<7+7>>
		                          , scale_t<1000000ll*1000000ll,1*1>
		                          , value_type >;
		using quantity3 = quantity< dimension< exponent_t<1+1+1>
		                                     , exponent_t<2+2+2>
		                                     , exponent_t<3+3+3>
		                                     , exponent_t<4+4+4>
		                                     , exponent_t<5+5+5>
		                                     , exponent_t<6+6+6>
		                                     , exponent_t<7+7+7>>
		                          , scale_t<1000000ll*1000000ll*1000000ll,1*1*1>
		                          , value_type >;
		using quantity_1 = quantity< dimension< exponent_t<-1>
		                                      , exponent_t<-2>
		                                      , exponent_t<-3>
		                                      , exponent_t<-4>
		                                      , exponent_t<-5>
		                                      , exponent_t<-6>
		                                      , exponent_t<-7>>
		                           , scale_t<1000000,1>
		                           , value_type >;
		using quantity_2 = quantity< dimension< exponent_t<-1-1>
		                                      , exponent_t<-2-2>
		                                      , exponent_t<-3-3>
		                                      , exponent_t<-4-4>
		                                      , exponent_t<-5-5>
		                                      , exponent_t<-6-6>
		                                      , exponent_t<-7-7>>
		                           , scale_t<1000000ll*1000000ll,1>
		                           , value_type >;

		quantity1 a{2};

		SUBCASE("integer exponents") {
			CHECK(typeid(unlib::pow< 3>(a)) == typeid(quantity3));
			CHECK(typeid(unlib::pow< 2>(a)) == typeid(quantity2));
			CHECK(typeid(unlib::pow< 1>(a)) == typeid(quantity1));
			CHECK(typeid(unlib::pow< 0>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<-1>(a)) == typeid(quantity_1));
			CHECK(typeid(unlib::pow<-2>(a)) == typeid(quantity_2));
		}
		SUBCASE("ratio exponents") {
			using quantity_r1  = quantity1;
			using quantity_r2  = quantity< dimension< exponent_t<1,2>
			                                        , exponent_t<2,2>
			                                        , exponent_t<3,2>
			                                        , exponent_t<4,2>
			                                        , exponent_t<5,2>
			                                        , exponent_t<6,2>
			                                        , exponent_t<7,2>>
			                             , scale_t<1000>
			                             , value_type >;
			using quantity_r3  = quantity< dimension< exponent_t<1,3>
			                                        , exponent_t<2,3>
			                                        , exponent_t<3,3>
			                                        , exponent_t<4,3>
			                                        , exponent_t<5,3>
			                                        , exponent_t<6,3>
			                                        , exponent_t<7,3>>
			                             , scale_t<100>
			                             , value_type >;
			using quantity_r_1 = quantity< dimension< exponent_t<-1>
			                                        , exponent_t<-2>
			                                        , exponent_t<-3>
			                                        , exponent_t<-4>
			                                        , exponent_t<-5>
			                                        , exponent_t<-6>
			                                        , exponent_t<-7>>
			                             , scale_t<1000000>
			                             , value_type >;
			using quantity_r_2 = quantity< dimension< exponent_t<-1,2>
			                                        , exponent_t<-2,2>
			                                        , exponent_t<-3,2>
			                                        , exponent_t<-4,2>
			                                        , exponent_t<-5,2>
			                                        , exponent_t<-6,2>
			                                        , exponent_t<-7,2>>
			                             , scale_t<1000>
			                             , value_type >;
			using quantity_r_3 = quantity< dimension< exponent_t<-1,3>
			                                        , exponent_t<-2,3>
			                                        , exponent_t<-3,3>
			                                        , exponent_t<-4,3>
			                                        , exponent_t<-5,3>
			                                        , exponent_t<-6,3>
			                                        , exponent_t<-7,3>>
			                              , scale_t<100>
			                              , value_type >;

			CHECK(typeid(unlib::pow<ratio_t< 3, 1>>(a)) == typeid(quantity3));
			CHECK(typeid(unlib::pow<ratio_t< 2, 1>>(a)) == typeid(quantity2));
			CHECK(typeid(unlib::pow<ratio_t< 1, 1>>(a)) == typeid(quantity1));
			CHECK(typeid(unlib::pow<ratio_t< 0, 1>>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<ratio_t<-1, 1>>(a)) == typeid(quantity_1));
			CHECK(typeid(unlib::pow<ratio_t<-2, 1>>(a)) == typeid(quantity_2));

			CHECK(typeid(unlib::pow<ratio_t< 1, 3>>(a)) == typeid(quantity_r3));
			CHECK(typeid(unlib::pow<ratio_t< 1, 2>>(a)) == typeid(quantity_r2));
			CHECK(typeid(unlib::pow<ratio_t< 1, 1>>(a)) == typeid(quantity_r1));
			CHECK(typeid(unlib::pow<ratio_t< 0, 1>>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<ratio_t< 1,-1>>(a)) == typeid(quantity_r_1));
			CHECK(typeid(unlib::pow<ratio_t< 1,-2>>(a)) == typeid(quantity_r_2));
			CHECK(typeid(unlib::pow<ratio_t< 1,-3>>(a)) == typeid(quantity_r_3));
		}
	}

	SUBCASE("sqrt") {
		using quantity_2 = quantity<dimension_a, scale_t<100,1>, value_type>;

		using dimension_sqrt = dimension<exponent_t<1,2>
		                                ,exponent_t<2,2>
		                                ,exponent_t<3,2>
		                                ,exponent_t<4,2>
		                                ,exponent_t<5,2>
		                                ,exponent_t<6,2>
		                                ,exponent_t<7,2>>;
		CHECK(typeid(unlib::sqrt_dimension_t<dimension_a>) == typeid(dimension_sqrt));

		using quantity_sqrt = quantity<dimension_sqrt, scale_t<10>, value_type>;
		CHECK(typeid(quantity_sqrt) == typeid(unlib::sqrt_quantity_t<quantity_2>));

		const auto sq_root_1 = unlib::sqrt(quantity_2{81});
		CHECK(typeid(sq_root_1) == typeid(quantity_sqrt));
		CHECK(sq_root_1.get() == doctest::Approx(9));

		const auto sq_root_2 = unlib::pow<unlib::ratio_t<1,2>>(quantity_2{81});
		CHECK(typeid(sq_root_2) == typeid(quantity_sqrt));
		CHECK(sq_root_2.get() == doctest::Approx(9));

		const auto sq_root_3 = unlib::pow<unlib::ratio_t<2,4>>(quantity_2{81});
		CHECK(typeid(sq_root_3) == typeid(quantity_sqrt));
		CHECK(sq_root_3.get() == doctest::Approx(9));
	}

	SUBCASE("cbrt") {
		using quantity_3 = quantity<dimension_a, scale_t<1000,1>, value_type>;

		using dimension_cbrt = dimension<exponent_t<1,3>
		                                ,exponent_t<2,3>
		                                ,exponent_t<3,3>
		                                ,exponent_t<4,3>
		                                ,exponent_t<5,3>
		                                ,exponent_t<6,3>
		                                ,exponent_t<7,3>>;
		CHECK(typeid(unlib::cbrt_dimension_t<dimension_a>) == typeid(dimension_cbrt));

		using quantity_cbrt = quantity<dimension_cbrt, scale_t<10>, value_type>;
		CHECK(typeid(quantity_cbrt) == typeid(unlib::cbrt_quantity_t<quantity_3>));

		const auto cb_root_1 = unlib::cbrt(quantity_3{64});
		CHECK(typeid(cb_root_1) == typeid(quantity_cbrt));
		CHECK(cb_root_1.get() == doctest::Approx(4));

		const auto cb_root_2 = unlib::pow<unlib::ratio_t<1,3>>(quantity_3{64});
		CHECK(typeid(cb_root_2) == typeid(quantity_cbrt));
		CHECK(cb_root_2.get() == doctest::Approx(4));

		const auto cb_root_3 = unlib::pow<unlib::ratio_t<3,9>>(quantity_3{64});
		CHECK(typeid(cb_root_3) == typeid(quantity_cbrt));
		CHECK(cb_root_3.get() == doctest::Approx(4));
	}
}
