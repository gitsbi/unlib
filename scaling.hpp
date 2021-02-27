#ifndef UNLIB_SCALING_HPP
#define UNLIB_SCALING_HPP

/*
 * scaling.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdint>

#include <unlib/ratio.hpp>

namespace unlib {

/**
 * @brief Quantity scale
 *
 * A scale is a rational fraction. Quantities have a scaling for their value
 * that is part of their type. Common scalings are the ones SI provides (kilo,
 * milli, etc.), but time spans are measured in weird scales which need their
 * own definitions.
 *
 * There are meta functions to scale by and to different scalings.
 */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using scale_t = ratio_t<Num,Den>;

/** an unscaled value */
using     no_scaling = scale_t<1>;

/**
 * @{
 * import std ratios so they are available in the unlib namespace
 */
using   atto_scaling = std:: atto;
using  femto_scaling = std::femto;
using   pico_scaling = std:: pico;
using   nano_scaling = std:: nano;
using  micro_scaling = std::micro;
using  milli_scaling = std::milli;
using  centi_scaling = std::centi;
using   deci_scaling = std:: deci;
using   deca_scaling = std:: deca;
using  hecto_scaling = std::hecto;
using   kilo_scaling = std:: kilo;
using   mega_scaling = std:: mega;
using   giga_scaling = std:: giga;
using   tera_scaling = std:: tera;
using   peta_scaling = std:: peta;
using    exa_scaling = std::  exa;
/** @} */

/**
 * @{
 * humans measure time in Babylonian multiples of seconds
 */
using second_scaling = no_scaling;
using minute_scaling = scale_t<60>;
using   hour_scaling = std::ratio_multiply<minute_scaling, scale_t<60>>;
using    day_scaling = std::ratio_multiply<  hour_scaling, scale_t<24>>;
using   week_scaling = std::ratio_multiply<   day_scaling, scale_t< 7>>;
/** @} */

namespace detail {

template<typename NewScale, typename T> struct scale_by;
template<typename NewScale, typename T> struct scale_to;

/* note Providing the implementations for scales here, allowing for other
 *      implementations for other types to be added to the detail namespace
 *      elsewhere. */
template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_by<NewScale, std::ratio<Num,Den>>        {using type = std::ratio_multiply<NewScale,std::ratio<Num,Den>>;};
template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_to<NewScale, std::ratio<Num,Den>>        {using type = NewScale;};

}

template<typename NewScale, typename T>
using scale_by_t = typename detail::scale_by<NewScale,T>::type;
template<typename NewScale, typename T>
using scale_to_t = typename detail::scale_to<NewScale,T>::type;

/**
 * @{
 *
 * Meta functions to get the type of a specific scalingy.
 *
 * @note micro<kilo<meter>> will result in milli<meter>, whereas
 *       to_micro<kilo<no_scaling>> and to_micro<to_kilo<no_scaling>> will
 *       result in micro<no_scaling>.
 *
 * @note With other implementations ("overloads) of detail::scale_by and
 *       detail::scale_to provided, these meta functions can be used to scale
 *       types other than scales, too.
 */
template<typename T> using atto            = scale_by_t<  atto_scaling,T>;
template<typename T> using femto           = scale_by_t< femto_scaling,T>;
template<typename T> using pico            = scale_by_t<  pico_scaling,T>;
template<typename T> using nano            = scale_by_t<  nano_scaling,T>;
template<typename T> using micro           = scale_by_t< micro_scaling,T>;
template<typename T> using milli           = scale_by_t< milli_scaling,T>;
template<typename T> using centi           = scale_by_t< centi_scaling,T>;
template<typename T> using deci            = scale_by_t<  deci_scaling,T>;
template<typename T> using no_scale        = T;
template<typename T> using deca            = scale_by_t<  deca_scaling,T>;
template<typename T> using hecto           = scale_by_t< hecto_scaling,T>;
template<typename T> using kilo            = scale_by_t<  kilo_scaling,T>;
template<typename T> using mega            = scale_by_t<  mega_scaling,T>;
template<typename T> using giga            = scale_by_t<  giga_scaling,T>;
template<typename T> using tera            = scale_by_t<  tera_scaling,T>;
template<typename T> using peta            = scale_by_t<  peta_scaling,T>;
template<typename T> using exa             = scale_by_t<   exa_scaling,T>;

template<typename T> using second_scale    = T;
template<typename T> using minute_scale    = scale_by_t<minute_scaling,T>;
template<typename T> using   hour_scale    = scale_by_t<  hour_scaling,T>;
template<typename T> using    day_scale    = scale_by_t<   day_scaling,T>;
template<typename T> using   week_scale    = scale_by_t<  week_scaling,T>;

template<typename T> using to_atto         = scale_to_t<  atto_scaling,T>;
template<typename T> using to_femto        = scale_to_t< femto_scaling,T>;
template<typename T> using to_pico         = scale_to_t<  pico_scaling,T>;
template<typename T> using to_nano         = scale_to_t<  nano_scaling,T>;
template<typename T> using to_micro        = scale_to_t< micro_scaling,T>;
template<typename T> using to_milli        = scale_to_t< milli_scaling,T>;
template<typename T> using to_centi        = scale_to_t< centi_scaling,T>;
template<typename T> using to_deci         = scale_to_t<  deci_scaling,T>;
template<typename T> using to_no_scale     = no_scale<T>;
template<typename T> using to_deca         = scale_to_t<  deca_scaling,T>;
template<typename T> using to_hecto        = scale_to_t< hecto_scaling,T>;
template<typename T> using to_kilo         = scale_to_t<  kilo_scaling,T>;
template<typename T> using to_mega         = scale_to_t<  mega_scaling,T>;
template<typename T> using to_giga         = scale_to_t<  giga_scaling,T>;
template<typename T> using to_tera         = scale_to_t<  tera_scaling,T>;
template<typename T> using to_peta         = scale_to_t<  peta_scaling,T>;
template<typename T> using to_exa          = scale_to_t<   exa_scaling,T>;

template<typename T> using to_second_scale = to_no_scale<T>;
template<typename T> using to_minute_scale = scale_to_t<minute_scaling,T>;
template<typename T> using to_hour_scale   = scale_to_t<  hour_scaling,T>;
template<typename T> using to_day_scale    = scale_to_t<   day_scaling,T>;
template<typename T> using to_week_scale   = scale_to_t<  week_scaling,T>;
/** @} */

/**
 * @{
 *
 * @brief Multiply and divide scale types
 */
template<typename Scale1, typename Scale2> using  mul_scale_t = std::ratio_multiply<Scale1,Scale2>;
template<typename Scale1, typename Scale2> using  div_scale_t = std::ratio_divide  <Scale1,Scale2>;
template<typename Scale , typename Ratio > using  pow_scale_t = ratio_root_t< ratio_pow_t< Scale
                                                                                         , abs_t<Ratio>::num >
                                                                            , abs_t<Ratio>::den >;
template<typename Scale                  > using sqrt_scale_t = ratio_root_t<Scale, 2>;
template<typename Scale                  > using cbrt_scale_t = ratio_root_t<Scale, 3>;
/** @} */

}

#endif //UNLIB_SCALING_HPP
