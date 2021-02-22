#include <unlib/limits.hpp>

#include <cinttypes>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

namespace {

using some_test_unit = unlib::unit_t<unlib::pow_unit_t<unlib::detail::            time_unit,std::ratio<1>>
                                    ,unlib::pow_unit_t<unlib::detail::            mass_unit,std::ratio<2>>
                                    ,unlib::pow_unit_t<unlib::detail::          length_unit,std::ratio<3>>
                                    ,unlib::pow_unit_t<unlib::detail::         current_unit,std::ratio<4>>
                                    ,unlib::pow_unit_t<unlib::detail::      luminosity_unit,std::ratio<5>>
                                    ,unlib::pow_unit_t<unlib::detail::     temperature_unit,std::ratio<6>>
                                    ,unlib::pow_unit_t<unlib::detail::substance_amount_unit,std::ratio<7>> >;

template<typename V>
using some_test_quantity = unlib::quantity<some_test_unit,unlib::no_scaling,V>;

using unlib::detail::is_floating_point_v;

template<typename F> std::enable_if_t<    is_floating_point_v<F>,bool> is_nan(F x)  {return x != x;}
template<typename T> std::enable_if_t<not is_floating_point_v<T>,bool> is_nan(T x)  {return false;}

}

TEST_CASE_TEMPLATE("numeric_limits", T
                                   , int8_t , uint8_t
                                   , int16_t, uint16_t
                                   , int32_t, uint32_t
                                   , int64_t, uint64_t
                                   , float, double, long double) {
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_specialized    == std::numeric_limits<T>::is_specialized    );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_signed         == std::numeric_limits<T>::is_signed         );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_integer        == std::numeric_limits<T>::is_integer        );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_exact          == std::numeric_limits<T>::is_exact          );
	CHECK( std::numeric_limits<some_test_quantity<T>>::has_infinity      == std::numeric_limits<T>::has_infinity      );
	CHECK( std::numeric_limits<some_test_quantity<T>>::has_quiet_NaN     == std::numeric_limits<T>::has_quiet_NaN     );
	CHECK( std::numeric_limits<some_test_quantity<T>>::has_signaling_NaN == std::numeric_limits<T>::has_signaling_NaN );
	CHECK( std::numeric_limits<some_test_quantity<T>>::has_denorm        == std::numeric_limits<T>::has_denorm        );
	CHECK( std::numeric_limits<some_test_quantity<T>>::has_denorm_loss   == std::numeric_limits<T>::has_denorm_loss   );
	CHECK( std::numeric_limits<some_test_quantity<T>>::round_style       == std::numeric_limits<T>::round_style       );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_iec559         == std::numeric_limits<T>::is_iec559         );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_bounded        == std::numeric_limits<T>::is_bounded        );
	CHECK( std::numeric_limits<some_test_quantity<T>>::is_modulo         == std::numeric_limits<T>::is_modulo         );
	CHECK( std::numeric_limits<some_test_quantity<T>>::digits            == std::numeric_limits<T>::digits            );
	CHECK( std::numeric_limits<some_test_quantity<T>>::digits10          == std::numeric_limits<T>::digits10          );
	CHECK( std::numeric_limits<some_test_quantity<T>>::max_digits10      == std::numeric_limits<T>::max_digits10      );
	CHECK( std::numeric_limits<some_test_quantity<T>>::radix             == std::numeric_limits<T>::radix             );
	CHECK( std::numeric_limits<some_test_quantity<T>>::min_exponent      == std::numeric_limits<T>::min_exponent      );
	CHECK( std::numeric_limits<some_test_quantity<T>>::min_exponent10    == std::numeric_limits<T>::min_exponent10    );
	CHECK( std::numeric_limits<some_test_quantity<T>>::max_exponent      == std::numeric_limits<T>::max_exponent      );
	CHECK( std::numeric_limits<some_test_quantity<T>>::max_exponent10    == std::numeric_limits<T>::max_exponent10    );
	CHECK( std::numeric_limits<some_test_quantity<T>>::traps             == std::numeric_limits<T>::traps             );
	CHECK( std::numeric_limits<some_test_quantity<T>>::tinyness_before   == std::numeric_limits<T>::tinyness_before   );

	CHECK( std::numeric_limits<some_test_quantity<T>>::min        ().get() == std::numeric_limits<T>::min        () );
	CHECK( std::numeric_limits<some_test_quantity<T>>::max        ().get() == std::numeric_limits<T>::max        () );
	CHECK( std::numeric_limits<some_test_quantity<T>>::lowest     ().get() == std::numeric_limits<T>::lowest     () );
	CHECK( std::numeric_limits<some_test_quantity<T>>::epsilon    ().get() == std::numeric_limits<T>::epsilon    () );
	CHECK( std::numeric_limits<some_test_quantity<T>>::round_error().get() == std::numeric_limits<T>::round_error() );
	CHECK( std::numeric_limits<some_test_quantity<T>>::infinity   ().get() == std::numeric_limits<T>::infinity   () );
	CHECK( std::numeric_limits<some_test_quantity<T>>::denorm_min ().get() == std::numeric_limits<T>::denorm_min () );

	CHECK( is_nan(std::numeric_limits<some_test_quantity<T>>::quiet_NaN    ().get()) == is_nan(std::numeric_limits<T>::quiet_NaN    ()) );
	CHECK( is_nan(std::numeric_limits<some_test_quantity<T>>::signaling_NaN().get()) == is_nan(std::numeric_limits<T>::signaling_NaN()) );
}
