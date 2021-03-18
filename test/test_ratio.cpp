#include <unlib/unlib/ratio.hpp>

#include <string>
#include <cmath>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

#include <unlib/unlib/scaling.hpp>

namespace {

using doctest::Approx;

constexpr double get_double(std::intmax_t num, std::intmax_t den) {return static_cast<double>(num)/static_cast<double>(den);}

template<std::intmax_t Num, std::intmax_t Den>
constexpr double get_double() {return static_cast<double>(Num)/static_cast<double>(Den);}
template<typename Ratio>
constexpr double get_double() {return get_double<Ratio::num,Ratio::den>();}

template<std::intmax_t Root> struct root;
template<> struct root<1> { static double get_root(std::intmax_t Num, std::intmax_t Den) {return           get_double(Num,Den) ;} };
template<> struct root<2> { static double get_root(std::intmax_t Num, std::intmax_t Den) {return std::sqrt(get_double(Num,Den));} };
template<> struct root<3> { static double get_root(std::intmax_t Num, std::intmax_t Den) {return std::cbrt(get_double(Num,Den));} };

template<std::intmax_t Root, std::intmax_t Num, std::intmax_t Den>
bool check_ratio_root() {
	CHECK( get_double<unlib::ratio_root_t<unlib::ratio_t<Num,Den>, Root>>() == Approx(root<Root>::get_root(Num,Den)) );
	return get_double<unlib::ratio_root_t<unlib::ratio_t<Num,Den>, Root>>() == Approx(root<Root>::get_root(Num,Den));
}

}

TEST_CASE("ratios") {

	SUBCASE("ratio_t") {
		CHECK(typeid(unlib::ratio_t<1,2>) == typeid(std::ratio<1,2>));
		CHECK(typeid(unlib::ratio_t<2,4>) == typeid(std::ratio<1,2>));
		CHECK(typeid(unlib::ratio_t<1,2>) != typeid(std::ratio<2,1>));
		CHECK(typeid(unlib::ratio_t<1,2>) != typeid(std::ratio<2,4>));
	}

	using unlib::ratio_t;

	SUBCASE("ratio_equal_v") {
		CHECK(    unlib::ratio_equal_v<ratio_t<1,2>,std::ratio<1,2>>);
		CHECK(    unlib::ratio_equal_v<ratio_t<2,4>,std::ratio<1,2>>);
		CHECK(    unlib::ratio_equal_v<ratio_t<1,2>,std::ratio<2,4>>);
		CHECK(not unlib::ratio_equal_v<ratio_t<1,2>,std::ratio<2,1>>);
	}

	SUBCASE("ratio_negate_t") {
		CHECK(typeid(unlib::ratio_negate_t<std::ratio< 1,2>>) == typeid(std::ratio<-1,2>));
		CHECK(typeid(unlib::ratio_negate_t<std::ratio< 2,4>>) == typeid(std::ratio<-1,2>));
		CHECK(typeid(unlib::ratio_negate_t<std::ratio<-1,2>>) == typeid(std::ratio< 1,2>));
		CHECK(typeid(unlib::ratio_negate_t<std::ratio<-2,4>>) == typeid(std::ratio< 1,2>));
	}

	SUBCASE("ratio_reciprocal_t") {
		CHECK(typeid(unlib::ratio_reciprocal_t<ratio_t<1,2>>) == typeid(std::ratio<2,1>));
		CHECK(typeid(unlib::ratio_reciprocal_t<ratio_t<2,4>>) == typeid(std::ratio<2,1>));
		CHECK(typeid(unlib::ratio_reciprocal_t<ratio_t<1,2>>) != typeid(std::ratio<1,2>));
		CHECK(typeid(unlib::ratio_reciprocal_t<ratio_t<1,2>>) != typeid(std::ratio<2,4>));
	}

	SUBCASE("power") {
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<0,1>, 0>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<0,1>, 1>) == typeid(ratio_t<0>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<0,1>, 2>) == typeid(ratio_t<0>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<0,1>, 3>) == typeid(ratio_t<0>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<0,1>, 4>) == typeid(ratio_t<0>));

		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>,-4>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>,-3>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>,-2>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>,-1>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>, 0>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>, 1>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>, 2>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>, 3>) == typeid(ratio_t<1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,1>, 4>) == typeid(ratio_t<1>));

		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>,-4>) == typeid(ratio_t< 1,16>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>,-3>) == typeid(ratio_t< 1, 8>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>,-2>) == typeid(ratio_t< 1, 4>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>,-1>) == typeid(ratio_t< 1, 2>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>, 0>) == typeid(ratio_t< 1, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>, 1>) == typeid(ratio_t< 2, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>, 2>) == typeid(ratio_t< 4, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>, 3>) == typeid(ratio_t< 8, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<2,1>, 4>) == typeid(ratio_t<16, 1>));

		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>,-4>) == typeid(ratio_t<16, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>,-3>) == typeid(ratio_t< 8, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>,-2>) == typeid(ratio_t< 4, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>,-1>) == typeid(ratio_t< 2, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>, 0>) == typeid(ratio_t< 1, 1>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>, 1>) == typeid(ratio_t< 1, 2>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>, 2>) == typeid(ratio_t< 1, 4>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>, 3>) == typeid(ratio_t< 1, 8>));
		CHECK( typeid(unlib::ratio_pow_t<ratio_t<1,2>, 4>) == typeid(ratio_t< 1,16>));
	}

	SUBCASE("root") {
		SUBCASE("basic cases") {
			CHECK( check_ratio_root<1,          1,         5>() );
			CHECK( check_ratio_root<1,          1,         4>() );
			CHECK( check_ratio_root<1,          1,         3>() );
			CHECK( check_ratio_root<1,          1,         2>() );
			CHECK( check_ratio_root<1,          1,         1>() );
			CHECK( check_ratio_root<1,          2,         1>() );
			CHECK( check_ratio_root<1,          3,         1>() );
			CHECK( check_ratio_root<1,          4,         1>() );
			CHECK( check_ratio_root<1,          5,         1>() );

			CHECK( check_ratio_root<1,         60,         1>() );
			CHECK( check_ratio_root<1,      60*60,         1>() );
			CHECK( check_ratio_root<1,   24*60*60,         1>() );
			CHECK( check_ratio_root<1, 7*24*60*60,         1>() );

			CHECK( check_ratio_root<1,          1,        60>() );
			CHECK( check_ratio_root<1,          1,     60*60>() );
			CHECK( check_ratio_root<1,          1,  24*60*60>() );
			CHECK( check_ratio_root<1,          1,7*24*60*60>() );
		}
		SUBCASE("square root") {
			CHECK( check_ratio_root<2,                      1,      10000000000000000>() );
			CHECK( check_ratio_root<2,                      1,        100000000000000>() );
			CHECK( check_ratio_root<2,                      1,          1000000000000>() );
			CHECK( check_ratio_root<2,                      1,            10000000000>() );
			CHECK( check_ratio_root<2,                      1,              100000000>() );
			CHECK( check_ratio_root<2,                      1,                1000000>() );
			CHECK( check_ratio_root<2,                      1,                  10000>() );
			CHECK( check_ratio_root<2,                      1,                    100>() );
			CHECK( check_ratio_root<2,                      1,                      1>() );
			CHECK( check_ratio_root<2,                    100,                      1>() );
			CHECK( check_ratio_root<2,                  10000,                      1>() );
			CHECK( check_ratio_root<2,                1000000,                      1>() );
			CHECK( check_ratio_root<2,              100000000,                      1>() );
			CHECK( check_ratio_root<2,            10000000000,                      1>() );
			CHECK( check_ratio_root<2,          1000000000000,                      1>() );
			CHECK( check_ratio_root<2,        100000000000000,                      1>() );
			CHECK( check_ratio_root<2,      10000000000000000,                      1>() );

			CHECK( check_ratio_root<2,        60ll*        60,                      1>() );
			CHECK( check_ratio_root<2,     60*60ll*     60*60,                      1>() );
			CHECK( check_ratio_root<2,  24*60*60ll*  24*60*60,                      1>() );
			CHECK( check_ratio_root<2,7*24*60*60ll*7*24*60*60,                      1>() );

			CHECK( check_ratio_root<2,                      1,        60ll*        60>() );
			CHECK( check_ratio_root<2,                      1,     60*60ll*     60*60>() );
			CHECK( check_ratio_root<2,                      1,  24*60*60ll*  24*60*60>() );
			CHECK( check_ratio_root<2,                      1,7*24*60*60ll*7*24*60*60>() );
		}
		SUBCASE("cube root") {
			CHECK( check_ratio_root<3,                                 1,                  1000000000000000>() );
			CHECK( check_ratio_root<3,                                 1,                     1000000000000>() );
			CHECK( check_ratio_root<3,                                 1,                        1000000000>() );
			CHECK( check_ratio_root<3,                                 1,                           1000000>() );
			CHECK( check_ratio_root<3,                                 1,                              1000>() );
			CHECK( check_ratio_root<3,                                 1,                                 1>() );
			CHECK( check_ratio_root<3,                              1000,                                 1>() );
			CHECK( check_ratio_root<3,                           1000000,                                 1>() );
			CHECK( check_ratio_root<3,                        1000000000,                                 1>() );
			CHECK( check_ratio_root<3,                     1000000000000,                                 1>() );
			CHECK( check_ratio_root<3,                  1000000000000000,                                 1>() );

			CHECK( check_ratio_root<3,        60ll*        60*        60,                                 1>() );
			CHECK( check_ratio_root<3,     60*60ll*     60*60*     60*60,                                 1>() );
			CHECK( check_ratio_root<3,  24*60*60ll*  24*60*60*  24*60*60,                                 1>() );
			CHECK( check_ratio_root<3,7*24*60*60ll*7*24*60*60*7*24*60*60,                                 1>() );

			CHECK( check_ratio_root<3,                                 1,        60ll*        60*        60>() );
			CHECK( check_ratio_root<3,                                 1,     60*60ll*     60*60*     60*60>() );
			CHECK( check_ratio_root<3,                                 1,  24*60*60ll*  24*60*60*  24*60*60>() );
			CHECK( check_ratio_root<3,                                 1,7*24*60*60ll*7*24*60*60*7*24*60*60>() );
		}
	}

}

