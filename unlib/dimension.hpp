#ifndef UNLIB_DIMENSION_HPP
#define UNLIB_DIMENSION_HPP

/*
 * dimension.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdint>
#include <type_traits>

#include <unlib/unlib/ratio.hpp>

namespace unlib {

/**
 * @brief an exponent of a basic unit of the SI unit system
 *
 * An exponent is a std::ratio. An exponent with the numerator set to zero
 * represents a basic unit that is not present.
 */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using exponent_t = ratio_t<Num,Den>;

/**
 * @brief A physical dimension
 *
 * A dimension is a set of exponents (std::ratio<>) representing the seven
 * basic physical units of the SI unit system. If a basic unit is not present
 * in a unit, the exponent's numerator is zero.
 *
 * @tparam              TimeExp  Exponent representing the               Time basic unit
 * @tparam              MassExp  Exponent representing the               Mass basic unit
 * @tparam            LengthExp  Exponent representing the             Length basic unit
 * @tparam           CurrentExp  Exponent representing the            Current basic unit
 * @tparam       TemperatureExp  Exponent representing the        Temperature basic unit
 * @tparam   SubstanceAmountExp  Exponent representing the    SubstanceAmount basic unit
 * @tparam LuminousIntensityExp  Exponent representing the luminous intensity basic unit
 *
 * @note Use dimension_t to define dimensions rather than using this template
 *       directly. The dimension_t meta function creates instances of this
 *       template with all the basic units always in the same order, no matter
 *       how many are provided in which order.
 */
template< typename              TimeExp
        , typename              MassExp
        , typename            LengthExp
        , typename           CurrentExp
        , typename       TemperatureExp
        , typename   SubstanceAmountExp
        , typename LuminousIntensityExp >
struct dimension;

template< std::intmax_t              TimeNum, std::intmax_t              TimeDen
        , std::intmax_t              MassNum, std::intmax_t              MassDen
        , std::intmax_t            LengthNum, std::intmax_t            LengthDen
        , std::intmax_t           CurrentNum, std::intmax_t           CurrentDen
        , std::intmax_t       TemperatureNum, std::intmax_t       TemperatureDen
        , std::intmax_t   SubstanceAmountNum, std::intmax_t   SubstanceAmountDen
        , std::intmax_t LuminousIntensityNum, std::intmax_t LuminousIntensityDen >
struct dimension< std::ratio<             TimeNum,             TimeDen>
                , std::ratio<             MassNum,             MassDen>
                , std::ratio<           LengthNum,           LengthDen>
                , std::ratio<          CurrentNum,          CurrentDen>
                , std::ratio<      TemperatureNum,      TemperatureDen>
                , std::ratio<  SubstanceAmountNum,  SubstanceAmountDen>
                , std::ratio<LuminousIntensityNum,LuminousIntensityDen> > {
	using                time_exponent = exponent_t<             TimeNum,              TimeDen>;
	using                mass_exponent = exponent_t<             MassNum,              MassDen>;
	using              length_exponent = exponent_t<           LengthNum,            LengthDen>;
	using             current_exponent = exponent_t<          CurrentNum,           CurrentDen>;
	using         temperature_exponent = exponent_t<      TemperatureNum,       TemperatureDen>;
	using    substance_amount_exponent = exponent_t<  SubstanceAmountNum,   SubstanceAmountDen>;
	using  luminous_intensity_exponent = exponent_t<LuminousIntensityNum, LuminousIntensityDen>;
};

/**
 * @{
 * @brief dimensions for the seven basic SI units
 *
 * These can be use to create dimensions by using the `dimension_t` meta function.
 *
@code
using velocity = dimension_t<length, reciprocal_dimension_t<time>>;
@endcode
 *
 * @note Users should not have to deal with this, as they mostly work with
 *       quantities, not with dimensions.
 */
using               time = dimension<exponent_t<1>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>>;
using               mass = dimension<exponent_t<0>, exponent_t<1>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>>;
using             length = dimension<exponent_t<0>, exponent_t<0>, exponent_t<1>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>>;
using            current = dimension<exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<1>, exponent_t<0>, exponent_t<0>, exponent_t<0>>;
using        temperature = dimension<exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<1>, exponent_t<0>, exponent_t<0>>;
using   substance_amount = dimension<exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<1>, exponent_t<0>>;
using luminous_intensity = dimension<exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<0>, exponent_t<1>>;
/** @} */

/**
 * @{
 *
 * @brief access dimension exponents
 */
template<typename Dimension> using               time_exponent_t = typename Dimension::              time_exponent;
template<typename Dimension> using               mass_exponent_t = typename Dimension::              mass_exponent;
template<typename Dimension> using             length_exponent_t = typename Dimension::            length_exponent;
template<typename Dimension> using            current_exponent_t = typename Dimension::           current_exponent;
template<typename Dimension> using        temperature_exponent_t = typename Dimension::       temperature_exponent;
template<typename Dimension> using   substance_amount_exponent_t = typename Dimension::  substance_amount_exponent;
template<typename Dimension> using luminous_intensity_exponent_t = typename Dimension::luminous_intensity_exponent;
/** @} */

/**
 * @brief a dimension-less dimension
 *
 * @see scalar
 */
using dimensionless = dimension< exponent_t<0>
                               , exponent_t<0>
                               , exponent_t<0>
                               , exponent_t<0>
                               , exponent_t<0>
                               , exponent_t<0>
                               , exponent_t<0> >;

namespace detail {

/**
 * @{
 *
 * meta functions for dimension type manipulations
 */
template<template<typename> class Operation, typename Dimension>
struct apply_unary;
template< template<typename> class Operation
        , typename Tim, typename Mas, typename Len, typename Cur, typename Tem, typename Sub, typename Lum >
struct apply_unary<Operation, dimension<Tim,Mas,Len,Cur,Tem,Sub,Lum>> {
	using type = dimension< Operation<Tim>
	                      , Operation<Mas>
	                      , Operation<Len>
	                      , Operation<Cur>
	                      , Operation<Tem>
	                      , Operation<Sub>
	                      , Operation<Lum> >;
};
template<template<typename> class Operation, typename Dimension>
using apply_unary_t = typename apply_unary<Operation, Dimension>::type;

template<template<typename,typename> class Operation, typename Dimension, typename Operand>
struct apply_binary;
template< template< typename,typename> class Operation
        , typename Tim, typename Mas, typename Len, typename Cur, typename Tem, typename Sub, typename Lum
        , std::intmax_t Nom, std::intmax_t Den>
struct apply_binary<Operation, dimension<Tim,Mas,Len,Cur,Tem,Sub,Lum>, std::ratio<Nom,Den>> {
	using type = dimension< Operation<Tim, std::ratio<Nom,Den>>
	                      , Operation<Mas, std::ratio<Nom,Den>>
	                      , Operation<Len, std::ratio<Nom,Den>>
	                      , Operation<Cur, std::ratio<Nom,Den>>
	                      , Operation<Tem, std::ratio<Nom,Den>>
	                      , Operation<Sub, std::ratio<Nom,Den>>
	                      , Operation<Lum, std::ratio<Nom,Den>> >;
};
template<template<typename,typename> class Operation
        , typename Tim1, typename Mas1, typename Len1, typename Cur1, typename Tem1, typename Sub1, typename Lum1
        , typename Tim2, typename Mas2, typename Len2, typename Cur2, typename Tem2, typename Sub2, typename Lum2 >
struct apply_binary<Operation, dimension<Tim1,Mas1,Len1,Cur1,Tem1,Sub1,Lum1>, dimension<Tim2,Mas2,Len2,Cur2,Tem2,Sub2,Lum2>>  {
	using type = dimension< Operation<Tim1, Tim2>
	                      , Operation<Mas1, Mas2>
	                      , Operation<Len1, Len2>
	                      , Operation<Cur1, Cur2>
	                      , Operation<Tem1, Tem2>
	                      , Operation<Sub1, Sub2>
	                      , Operation<Lum1, Lum2> >;
};
template<template<typename,typename> class Operation, typename Dimension, typename Operand>
using apply_binary_t = typename apply_binary<Operation, Dimension, Operand>::type;

template<typename Dimension, typename Ratio>
struct pow_dimension;
template<typename Dimension, std::intmax_t Nom, std::intmax_t Den>
struct pow_dimension<Dimension, std::ratio<Nom,Den>> {
	using type = typename apply_binary<std::ratio_multiply, Dimension, std::ratio<Nom,Den>>::type;
};
/** @} */

}

/**
 * @{
 *
 * @brief common dimension type manipulations
 */
template<typename Dimension1, typename Dimension2> using        mul_dimension_t = detail::apply_binary_t<std::ratio_add     ,Dimension1,Dimension2>;
template<typename Dimension1, typename Dimension2> using        div_dimension_t = detail::apply_binary_t<std::ratio_subtract,Dimension1,Dimension2>;
template<typename Dimension , typename Ratio     > using        pow_dimension_t = detail::apply_binary_t<std::ratio_multiply,Dimension ,Ratio>;
template<typename Dimension>                       using     square_dimension_t = pow_dimension_t<Dimension,std::ratio<2,1>>;
template<typename Dimension>                       using       cube_dimension_t = pow_dimension_t<Dimension,std::ratio<3,1>>;
template<typename Dimension>                       using reciprocal_dimension_t = detail::apply_unary_t<ratio_negate_t,Dimension>;
template<typename Dimension>                       using       sqrt_dimension_t = pow_dimension_t<Dimension, std::ratio<1,2>>;
template<typename Dimension>                       using       cbrt_dimension_t = pow_dimension_t<Dimension, std::ratio<1,3>>;
/** @} */

/**
 * @{
 *
 * @brief check if two dimensions are compatible
 *
 * Dimensions are compatible if all their basic units have matching exponents.
 */
template<typename Dimension1, typename Dimension2>
using are_dimensions_compatible = std::integral_constant< bool
                                                        , ratio_equal_v<              time_exponent_t<Dimension1>,               time_exponent_t<Dimension2>>
                                                       && ratio_equal_v<              mass_exponent_t<Dimension1>,               mass_exponent_t<Dimension2>>
                                                       && ratio_equal_v<            length_exponent_t<Dimension1>,             length_exponent_t<Dimension2>>
                                                       && ratio_equal_v<           current_exponent_t<Dimension1>,            current_exponent_t<Dimension2>>
                                                       && ratio_equal_v<       temperature_exponent_t<Dimension1>,        temperature_exponent_t<Dimension2>>
                                                       && ratio_equal_v<  substance_amount_exponent_t<Dimension1>,   substance_amount_exponent_t<Dimension2>>
                                                       && ratio_equal_v<luminous_intensity_exponent_t<Dimension1>, luminous_intensity_exponent_t<Dimension2>> >;

template<typename Dimension1, typename Dimension2>
constexpr bool are_dimensions_compatible_v = are_dimensions_compatible<Dimension1,Dimension2>::value;
/** @} */

}

#endif /* UNLIB_DIMENSION_HPP */
