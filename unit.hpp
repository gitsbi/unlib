#ifndef UNLIB_UNIT_HPP
#define UNLIB_UNIT_HPP

/*
 * unit.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdint>
#include <type_traits>

#include <unlib/ratio.hpp>

namespace unlib {

/**
 * @brief an exponent of a basic SI unit
 *
 * An exponent with the numerator set to zero represents a unit that is not
 * present.
 */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using exponent_t = ratio_t<Num,Den>;

/**
 * @brief A physical unit
 *
 * A unit is a set of exponents (std::ratio<>) representing the seven basic
 * physical units of the SI unit system. If a basic unit is not present in a
 * unit, the exponent's numerator is zero.
 *
 * @tparam            TimeExp  Exponent representing the            Time basic unit
 * @tparam            MassExp  Exponent representing the            Mass basic unit
 * @tparam          LengthExp  Exponent representing the          Length basic unit
 * @tparam         CurrentExp  Exponent representing the         Current basic unit
 * @tparam      LuminosityExp  Exponent representing the      Luminosity basic unit
 * @tparam     TemperatureExp  Exponent representing the     Temperature basic unit
 * @tparam SubstanceAmountExp  Exponent representing the SubstanceAmount basic unit
 *
 * @note Use unit_t to define units rather than using this template drectly.
 *       The unit_t meta function creates instances of this template with all
 *       the basic units always in the same order, no matter in which order
 *       they are specified and how many are provided.
 */
template< typename            TimeExp
        , typename            MassExp
        , typename          LengthExp
        , typename         CurrentExp
        , typename      LuminosityExp
        , typename     TemperatureExp
        , typename SubstanceAmountExp >
struct unit;

template< std::intmax_t            TimeNum, std::intmax_t            TimeDen
        , std::intmax_t            MassNum, std::intmax_t            MassDen
        , std::intmax_t          LengthNum, std::intmax_t          LengthDen
        , std::intmax_t         CurrentNum, std::intmax_t         CurrentDen
        , std::intmax_t      LuminosityNum, std::intmax_t      LuminosityDen
        , std::intmax_t     TemperatureNum, std::intmax_t     TemperatureDen
        , std::intmax_t SubstanceAmountNum, std::intmax_t SubstanceAmountDen >
struct unit< std::ratio<           TimeNum,           TimeDen>
           , std::ratio<           MassNum,           MassDen>
           , std::ratio<         LengthNum,         LengthDen>
           , std::ratio<        CurrentNum,        CurrentDen>
           , std::ratio<     LuminosityNum,     LuminosityDen>
           , std::ratio<    TemperatureNum,    TemperatureDen>
           , std::ratio<SubstanceAmountNum,SubstanceAmountDen> > {
	using             time_exponent = exponent_t<            TimeNum,            TimeDen>;
	using             mass_exponent = exponent_t<            MassNum,            MassDen>;
	using           length_exponent = exponent_t<          LengthNum,          LengthDen>;
	using          current_exponent = exponent_t<         CurrentNum,         CurrentDen>;
	using       luminosity_exponent = exponent_t<      LuminosityNum,      LuminosityDen>;
	using      temperature_exponent = exponent_t<     TemperatureNum,     TemperatureDen>;
	using substance_amount_exponent = exponent_t< SubstanceAmountNum, SubstanceAmountDen>;
};

/**
 * @{
 *
 * @brief access unit exponents
 */
template<typename Unit> using             time_exponent_t = typename Unit::            time_exponent;
template<typename Unit> using             mass_exponent_t = typename Unit::            mass_exponent;
template<typename Unit> using           length_exponent_t = typename Unit::          length_exponent;
template<typename Unit> using          current_exponent_t = typename Unit::         current_exponent;
template<typename Unit> using       luminosity_exponent_t = typename Unit::      luminosity_exponent;
template<typename Unit> using      temperature_exponent_t = typename Unit::     temperature_exponent;
template<typename Unit> using substance_amount_exponent_t = typename Unit::substance_amount_exponent;
/** @} */

namespace detail {

/**
 * @{
 *
 * Shortcuts to make the following code more easily readable
 */
using e0 = exponent_t<0>;
using e1 = exponent_t<1>;
using e2 = exponent_t<2>;
/** @} */

/**
 * @{
 * @brief the seven basic SI units
 *
 * @note These are defined here, within `detail`, only because they are used
 *       here in order to access the exponents in unit in the following code.
 *       Definitions intended to be used by users are done elsewhere together
 *       with units derived from the basic units.
 */
using             time = unit< e1, e0, e0, e0, e0, e0, e0 >;
using             mass = unit< e0, e1, e0, e0, e0, e0, e0 >;
using           length = unit< e0, e0, e1, e0, e0, e0, e0 >;
using          current = unit< e0, e0, e0, e1, e0, e0, e0 >;
using       luminosity = unit< e0, e0, e0, e0, e1, e0, e0 >;
using      temperature = unit< e0, e0, e0, e0, e0, e1, e0 >;
using substance_amount = unit< e0, e0, e0, e0, e0, e0, e1 >;
/** @} */

/**
 * @{
 *
 * access the exponent for a specific basic unit in a unit
 */
template<typename Unit, typename BasicUnit> struct get_exponent;
template<typename Unit                    > struct get_exponent<Unit,            time> { using type =             time_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,            mass> { using type =             mass_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,          length> { using type =           length_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,         current> { using type =          current_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,      luminosity> { using type =       luminosity_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,     temperature> { using type =      temperature_exponent_t<Unit>; };
template<typename Unit                    > struct get_exponent<Unit,substance_amount> { using type = substance_amount_exponent_t<Unit>; };

template<typename Unit, typename BasicUnit>
using get_exponent_t = typename get_exponent<Unit,BasicUnit>::type;
/** @} */

/** an unsorted compile-time list of basic units */
template<typename ...BasicUnits>
struct unsorted_unit_list {};

/**
 * @{
 * @brief find the exponent of a basic unit in an unsorted_unit_list
 *
 * This searches the list for the first unit where the exponent of the basic
 * unit given is nonzero. If there is none, it return an exponent<0>.
 *
 * @note For this to be useful, the list must contain only basic units, and
 *       no two of these must be of the same type.
 */
template<typename BasicUnit, typename BasicList>
struct find_first_exponent;
template<typename BasicUnit, typename BasicList>
using find_first_exponent_t = typename find_first_exponent<BasicUnit,BasicList>::type;

template<typename BasicUnit>
struct find_first_exponent<BasicUnit,unsorted_unit_list<>> {
	using type = exponent_t<0>;
};
template<typename BasicUnit, typename Head, typename ...Tail>
struct find_first_exponent<BasicUnit,unsorted_unit_list<Head,Tail...>> {
	using type = std::conditional_t< get_exponent_t<Head,BasicUnit>::num != 0
	                               , get_exponent_t<Head,BasicUnit>
	                               , find_first_exponent_t<BasicUnit, unsorted_unit_list<Tail...>> >;
};
/** @} */

/** create a unit with the basic units being sorted into the correct order from a unsorted_unit_list */
template< typename UnsortedUnitList >
using create_unit_t = unit< typename find_first_exponent<            time, UnsortedUnitList>::type
                          , typename find_first_exponent<            mass, UnsortedUnitList>::type
                          , typename find_first_exponent<          length, UnsortedUnitList>::type
                          , typename find_first_exponent<         current, UnsortedUnitList>::type
                          , typename find_first_exponent<      luminosity, UnsortedUnitList>::type
                          , typename find_first_exponent<     temperature, UnsortedUnitList>::type
                          , typename find_first_exponent<substance_amount, UnsortedUnitList>::type >;

/**
 * @{
 *
 * meta functions for unit type manipulations
 */
template<template<typename> class Operation, typename Unit>
struct apply_unary;
template< template<typename> class Operation
        , typename Tim, typename Mas, typename Len, typename Cur, typename Lum, typename Tem, typename Sub >
struct apply_unary<Operation, unit<Tim,Mas,Len,Cur,Lum,Tem,Sub>> {
	using type = unit< Operation<Tim>
	                 , Operation<Mas>
	                 , Operation<Len>
	                 , Operation<Cur>
	                 , Operation<Lum>
	                 , Operation<Tem>
	                 , Operation<Sub> >;
};

template<template<typename,typename> class Operation, typename Unit, typename Operand>
struct apply_binary;
template< template< typename,typename> class Operation
        , typename Tim, typename Mas, typename Len, typename Cur, typename Lum, typename Tem, typename Sub
        , std::intmax_t Nom, std::intmax_t Den>
struct apply_binary<Operation, unit<Tim,Mas,Len,Cur,Lum,Tem,Sub>, std::ratio<Nom,Den>> {
	using type = unit< Operation<Tim, std::ratio<Nom,Den>>
	                 , Operation<Mas, std::ratio<Nom,Den>>
	                 , Operation<Len, std::ratio<Nom,Den>>
	                 , Operation<Cur, std::ratio<Nom,Den>>
	                 , Operation<Lum, std::ratio<Nom,Den>>
	                 , Operation<Tem, std::ratio<Nom,Den>>
	                 , Operation<Sub, std::ratio<Nom,Den>> >;
};
template<template<typename,typename> class Operation
        , typename Tim1, typename Mas1, typename Len1, typename Cur1, typename Lum1, typename Tem1, typename Sub1
        , typename Tim2, typename Mas2, typename Len2, typename Cur2, typename Lum2, typename Tem2, typename Sub2 >
struct apply_binary<Operation, unit<Tim1,Mas1,Len1,Cur1,Lum1,Tem1,Sub1>, unit<Tim2,Mas2,Len2,Cur2,Lum2,Tem2,Sub2>>  {
	using type = unit< Operation<Tim1, Tim2>
	                 , Operation<Mas1, Mas2>
	                 , Operation<Len1, Len2>
	                 , Operation<Cur1, Cur2>
	                 , Operation<Lum1, Lum2>
	                 , Operation<Tem1, Tem2>
	                 , Operation<Sub1, Sub2> >;
};

template<typename Unit, typename Ratio>
struct pow_unit;
template<typename Unit, std::intmax_t Nom, std::intmax_t Den>
struct pow_unit<Unit, std::ratio<Nom,Den>> {
	using type = typename apply_binary<std::ratio_multiply, Unit, std::ratio<Nom,Den>>::type;
};

using cube_unit = unit<e2, e2, e2, e2, e2, e2, e2>;
/** @} */

}

/** a dimension-less unit */
using dimensionless = unit< exponent_t<0>
                          , exponent_t<0>
                          , exponent_t<0>
                          , exponent_t<0>
                          , exponent_t<0>
                          , exponent_t<0>
                          , exponent_t<0> >;

/**
 * @brief create a unit type
 *
 * This is a meta function to create a unit with a correctly sorted basic
 * unit list from basic units.
 *
 * @note The result of this meta function is undefined if the arguments are
 *       not 7 or less different basic units.
 */
template< typename Basic1 = dimensionless
        , typename Basic2 = dimensionless
        , typename Basic3 = dimensionless
        , typename Basic4 = dimensionless
        , typename Basic5 = dimensionless
        , typename Basic6 = dimensionless
        , typename Basic7 = dimensionless >
using unit_t = detail::create_unit_t<detail::unsorted_unit_list<Basic1,Basic2,Basic3,Basic4,Basic5,Basic6,Basic7>>;

/**
 * @{
 *
 * @brief unit type manipulations
 */
template<typename Unit1, typename Unit2> using        mul_unit_t = typename detail::apply_binary<std::ratio_add     ,Unit1,Unit2>::type;
template<typename Unit1, typename Unit2> using        div_unit_t = typename detail::apply_binary<std::ratio_subtract,Unit1,Unit2>::type;
template<typename Unit , typename Ratio> using        pow_unit_t = typename detail::apply_binary<std::ratio_multiply,Unit ,Ratio>::type;
template<typename Unit>                  using     square_unit_t = pow_unit_t<Unit,std::ratio<2,1>>;
template<typename Unit>                  using       cube_unit_t = pow_unit_t<Unit,std::ratio<3,1>>;
template<typename Unit>                  using reciprocal_unit_t = typename detail::apply_unary<ratio_negate_t,Unit>::type;
template<typename Unit>                  using       sqrt_unit_t = pow_unit_t<Unit, std::ratio<1,2>>;
template<typename Unit>                  using       cbrt_unit_t = pow_unit_t<Unit, std::ratio<1,3>>;
/** @} */

/**
 * @{
 *
 * @brief check if two units are compatible
 *
 * Units are compatible if all their dimensions have matching exponents.
 */
template<typename Unit1, typename Unit2>
using are_units_compatible = std::integral_constant< bool
                                                   , ratio_equal_v<            time_exponent_t<Unit1>,             time_exponent_t<Unit2>>
                                                  && ratio_equal_v<            mass_exponent_t<Unit1>,             mass_exponent_t<Unit2>>
                                                  && ratio_equal_v<          length_exponent_t<Unit1>,           length_exponent_t<Unit2>>
                                                  && ratio_equal_v<         current_exponent_t<Unit1>,          current_exponent_t<Unit2>>
                                                  && ratio_equal_v<      luminosity_exponent_t<Unit1>,       luminosity_exponent_t<Unit2>>
                                                  && ratio_equal_v<     temperature_exponent_t<Unit1>,      temperature_exponent_t<Unit2>>
                                                  && ratio_equal_v<substance_amount_exponent_t<Unit1>, substance_amount_exponent_t<Unit2>> >;

template<typename Unit1, typename Unit2>
constexpr bool are_units_compatible_v = are_units_compatible<Unit1,Unit2>::value;
/** @} */

/**
 * @{
 *
 * @brief check if a unit is dimensionless
 *
 * A unit has no dimensions when all its basics' exponents' numerators are zero.
 */
template<typename Unit>
using is_unit_dimensionless = std::is_same<Unit,dimensionless>;

template<typename Unit>
constexpr bool is_unit_dimensionless_v = is_unit_dimensionless<Unit>::value;
/** @} */

}

#endif /* UNLIB_UNIT_HPP */
