#ifndef UNLIB_RATIO_HPP
#define UNLIB_RATIO_HPP

/*
 * ratio.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdint>
#include <ratio>

namespace unlib {

/**
 * @brief an exponent of a basic SI unit
 *
 * An exponent with the numerator set to zero represents a unit that is not
 * present.
 */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using ratio_t = typename std::ratio<Num,Den>::type;

/**
 * @brief Checks whether two ratios are equal
 *
 * @tparam Exp1, Exp2  Ratios to check for equality
 */
template<typename Exp1, typename Exp2>
constexpr bool ratio_equal_v = std::ratio_equal<Exp1,Exp2>::value;

/**
 * @{
 *
 * @brief Negates a ratio
 *
 * This will negate the numerator of the ratio
 *
 * @tparam Ratio
 */
template<typename Ratio>
struct ratio_negate {using type = ratio_t<-Ratio::num,Ratio::den>;};
template<typename Ratio>
using ratio_negate_t = typename ratio_negate<Ratio>::type;
/** @} */

/**
 * @{
 *
 * @brief Get the reciprocal of a ratio
 *
 * @tparam Ratio  Ratio to get reciprocal of
 */
template<typename Ratio>
struct ratio_reciprocal {using type = ratio_t<Ratio::den,Ratio::num>;};
template<typename Ratio>
using ratio_reciprocal_t = typename ratio_reciprocal<Ratio>::type;
/** @} */

namespace detail {

template<int Number, bool = (Number<0) > struct sign;
template<int Number>                     struct sign<Number,false> { static const int value = +1; };
template<int Number>                     struct sign<Number,true > { static const int value = -1; };
template<>                               struct sign<0     ,false> { static const int value =  0; };

template<typename Ratio, int Power, int Sign = sign<Power>::value>
struct ratio_pow;

template<typename Ratio, int Power>
using ratio_pow_t = typename ratio_pow<Ratio,Power>::type;

template<typename Ratio , int Sign > struct ratio_pow<     Ratio     ,     0, Sign> {using type = ratio_t<1>;};
template<std::intmax_t D, int Power> struct ratio_pow<std::ratio<0,D>, Power,    1> {using type = ratio_t<0>;};
template<std::intmax_t D, int Sign > struct ratio_pow<std::ratio<0,D>,     0, Sign> {using type = ratio_t<1>;};
template<typename Ratio , int Power> struct ratio_pow<     Ratio     , Power,   +1> {using type = std::ratio_multiply<Ratio, ratio_pow_t<Ratio,Power-1>>;};
template<typename Ratio , int Power> struct ratio_pow<     Ratio     , Power,   -1> {using type = ratio_pow_t<ratio_reciprocal_t<Ratio>, -Power>;};

namespace ratio_impl {

template<std::intmax_t Number, int Root>
struct root;

template<std::intmax_t Number          > struct root<Number,   1> {static constexpr std::intmax_t value =     Number;};
template<                      int Root> struct root<     1,Root> {static constexpr std::intmax_t value =          1;};
template<                              > struct root<     1,   1> {static constexpr std::intmax_t value =          1;};

template<> struct root<                    100              , 2 > {static constexpr std::intmax_t value =         10;};
template<> struct root<                  10000              , 2 > {static constexpr std::intmax_t value =        100;};
template<> struct root<                1000000              , 2 > {static constexpr std::intmax_t value =       1000;};
template<> struct root<              100000000              , 2 > {static constexpr std::intmax_t value =      10000;};
template<> struct root<            10000000000              , 2 > {static constexpr std::intmax_t value =     100000;};
template<> struct root<          1000000000000              , 2 > {static constexpr std::intmax_t value =    1000000;};
template<> struct root<        100000000000000              , 2 > {static constexpr std::intmax_t value =   10000000;};
template<> struct root<      10000000000000000              , 2 > {static constexpr std::intmax_t value =  100000000;};
template<> struct root<    1000000000000000000              , 2 > {static constexpr std::intmax_t value = 1000000000;};

template<> struct root<                   1000              , 3 > {static constexpr std::intmax_t value =         10;};
template<> struct root<                1000000              , 3 > {static constexpr std::intmax_t value =        100;};
template<> struct root<             1000000000              , 3 > {static constexpr std::intmax_t value =       1000;};
template<> struct root<          1000000000000              , 3 > {static constexpr std::intmax_t value =      10000;};
template<> struct root<       1000000000000000              , 3 > {static constexpr std::intmax_t value =     100000;};
template<> struct root<    1000000000000000000              , 3 > {static constexpr std::intmax_t value =    1000000;};

template<> struct root<        60ll*        60              , 2 > {static constexpr std::intmax_t value =         60;};
template<> struct root<     60*60ll*     60*60              , 2 > {static constexpr std::intmax_t value =      60*60;};
template<> struct root<  24*60*60ll*  24*60*60              , 2 > {static constexpr std::intmax_t value =   24*60*60;};
template<> struct root<7*24*60*60ll*7*24*60*60              , 2 > {static constexpr std::intmax_t value = 7*24*60*60;};

template<> struct root<        60ll*        60*        60   , 3 > {static constexpr std::intmax_t value =         60;};
template<> struct root<     60*60ll*     60*60*     60*60   , 3 > {static constexpr std::intmax_t value =      60*60;};
template<> struct root<  24*60*60ll*  24*60*60*  24*60*60   , 3 > {static constexpr std::intmax_t value =   24*60*60;};
template<> struct root<7*24*60*60ll*7*24*60*60*7*24*60*60   , 3 > {static constexpr std::intmax_t value = 7*24*60*60;};

}

template<typename Ratio, int Root>
using ratio_root_t = ratio_t< ratio_impl::root<Ratio::num,Root>::value
                            , ratio_impl::root<Ratio::den,Root>::value >;
}

/**
 * @brief Get the absolute value of a ratio
 *
 * @tparam Ratio  Ratio to get absolute value of
 */
template<typename Ratio>
using abs_t = std::conditional_t< Ratio::type::num < 0, ratio_t<-Ratio::type::num,Ratio::type::den>, Ratio>;

/**
 * @{
 *
 * @brief Get the power or root of a ratio
 *
 * @tparam Ratio  Ratio to get power of root of
 * @tparam Power  Power to get
 * @tparam  Root  Root to get
 *
 * @note This will not get results for any root of any ratio. Currently
 *       supported are the (0th, 1st,) 2nd, and 3rd root, but only if both the
 *       the numerator or the denominator is either a power of ten, or of one
 *       of the ratios needed to support time scales (60, 60*60, 24*60*60,
 *       7*24*60*60, of which the root is a rational number.
 *       Any other invocation will result in a compile error.
 */
template<typename Ratio, int Power> using ratio_pow_t  = detail::ratio_pow_t<Ratio,Power>;
template<typename Ratio, int Root > using ratio_root_t = detail::ratio_root_t<Ratio,Root>;
/** @} */

}

#endif //UNLIB_RATIO_HPP
