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
#include <ratio>

namespace unlib {

/**
 * @brief Quantity scale
 *
 * Quantities have a scaling for their value that is part of their type.
 * Common scalings are the ones SI provides (kilo, milli, etc.), but time
 * spans are measured in weird scales which need their own definitions.
 *
 * There are meta functions to scale by and to different scalings.
 */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using scale_t = typename std::ratio<Num,Den>::type;

/**
 * @{
 *
 * @brief Multiply and divide scale types
 */
template<typename Scale1, typename Scale2> using mul_scale_t = std::ratio_multiply<Scale1,Scale2>;
template<typename Scale1, typename Scale2> using div_scale_t = std::ratio_divide  <Scale1,Scale2>;
/** @} */

/** an unscaled value */
using     no_scaling = scale_t<1>;

/**
 * @{
 * import std ratios so they are available in the same namespace as no_scaling is
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
 * humans measure time in odd multiples of seconds
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
 *      implementations for other types to be added to this namespace
 *      elsewhere. */
template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_by<NewScale, std::ratio<Num,Den>>                  {using result = std::ratio_multiply<NewScale,std::ratio<Num,Den>>;};
template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_to<NewScale, std::ratio<Num,Den>>                  {using result = NewScale;};

}

/**
 * @{
 *
 * Meta functions to get the type of a specific scalingy.
 *
 * @note micro<kilo<meter>> will result in milli<meter>,
 *       to_micro<kilo<no_scaling>> and to_micro<to_kilo<no_scaling>> will
 *       result in micro<no_scaling>.
 *
 * @note With other implementations ("overloads) of detail::scale_by and
 *       detail::scale_to provided, these meta functions can be used to scale
 *       types other than scales, too.
 */
template<typename T> using atto            = typename detail::scale_by<  atto_scaling,T>::result;
template<typename T> using femto           = typename detail::scale_by< femto_scaling,T>::result;
template<typename T> using pico            = typename detail::scale_by<  pico_scaling,T>::result;
template<typename T> using nano            = typename detail::scale_by<  nano_scaling,T>::result;
template<typename T> using micro           = typename detail::scale_by< micro_scaling,T>::result;
template<typename T> using milli           = typename detail::scale_by< milli_scaling,T>::result;
template<typename T> using centi           = typename detail::scale_by< centi_scaling,T>::result;
template<typename T> using deci            = typename detail::scale_by<  deci_scaling,T>::result;
template<typename T> using no_scale        = T;
template<typename T> using deca            = typename detail::scale_by<  deca_scaling,T>::result;
template<typename T> using hecto           = typename detail::scale_by< hecto_scaling,T>::result;
template<typename T> using kilo            = typename detail::scale_by<  kilo_scaling,T>::result;
template<typename T> using mega            = typename detail::scale_by<  mega_scaling,T>::result;
template<typename T> using giga            = typename detail::scale_by<  giga_scaling,T>::result;
template<typename T> using tera            = typename detail::scale_by<  tera_scaling,T>::result;
template<typename T> using peta            = typename detail::scale_by<  peta_scaling,T>::result;
template<typename T> using exa             = typename detail::scale_by<   exa_scaling,T>::result;

template<typename T> using second_scale    = T;
template<typename T> using minute_scale    = typename detail::scale_by<minute_scaling,T>::result;
template<typename T> using   hour_scale    = typename detail::scale_by<  hour_scaling,T>::result;
template<typename T> using    day_scale    = typename detail::scale_by<   day_scaling,T>::result;
template<typename T> using   week_scale    = typename detail::scale_by<  week_scaling,T>::result;

template<typename T> using to_atto         = typename detail::scale_to<  atto_scaling,T>::result;
template<typename T> using to_femto        = typename detail::scale_to< femto_scaling,T>::result;
template<typename T> using to_pico         = typename detail::scale_to<  pico_scaling,T>::result;
template<typename T> using to_nano         = typename detail::scale_to<  nano_scaling,T>::result;
template<typename T> using to_micro        = typename detail::scale_to< micro_scaling,T>::result;
template<typename T> using to_milli        = typename detail::scale_to< milli_scaling,T>::result;
template<typename T> using to_centi        = typename detail::scale_to< centi_scaling,T>::result;
template<typename T> using to_deci         = typename detail::scale_to<  deci_scaling,T>::result;
template<typename T> using to_no_scale     = no_scale<T>;
template<typename T> using to_deca         = typename detail::scale_to<  deca_scaling,T>::result;
template<typename T> using to_hecto        = typename detail::scale_to< hecto_scaling,T>::result;
template<typename T> using to_kilo         = typename detail::scale_to<  kilo_scaling,T>::result;
template<typename T> using to_mega         = typename detail::scale_to<  mega_scaling,T>::result;
template<typename T> using to_giga         = typename detail::scale_to<  giga_scaling,T>::result;
template<typename T> using to_tera         = typename detail::scale_to<  tera_scaling,T>::result;
template<typename T> using to_peta         = typename detail::scale_to<  peta_scaling,T>::result;
template<typename T> using to_exa          = typename detail::scale_to<   exa_scaling,T>::result;

template<typename T> using to_second_scale = to_no_scale<T>;
template<typename T> using to_minute_scale = typename detail::scale_to<minute_scaling,T>::result;
template<typename T> using to_hour_scale   = typename detail::scale_to<  hour_scaling,T>::result;
template<typename T> using to_day_scale    = typename detail::scale_to<   day_scaling,T>::result;
template<typename T> using to_week_scale   = typename detail::scale_to<  week_scaling,T>::result;
/** @} */

}

#endif //UNLIB_SCALING_HPP
