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
using exponent = typename std::ratio<Num,Den>::type;

namespace detail {
/* workaround for C++14 lacking the ...v variants */
template<typename Exp1, typename Exp2>
constexpr bool ratio_equal_v = std::ratio_equal<Exp1,Exp2>::value;
}

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
 * @note Use create_unit_t to define units rather than using this template
 *       directly. The create_unit_t meta function creates instances of this
 *       template with all the basic units always in the same order, no
 *       matter in which order they are specified and how many are provided.
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
	using             time_exponent = std::ratio<            TimeNum,            TimeDen>;
	using             mass_exponent = std::ratio<            MassNum,            MassDen>;
	using           length_exponent = std::ratio<          LengthNum,          LengthDen>;
	using          current_exponent = std::ratio<         CurrentNum,         CurrentDen>;
	using       luminosity_exponent = std::ratio<      LuminosityNum,      LuminosityDen>;
	using      temperature_exponent = std::ratio<     TemperatureNum,     TemperatureDen>;
	using substance_amount_exponent = std::ratio< SubstanceAmountNum, SubstanceAmountDen>;
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
using e0 = exponent<0>;
using e1 = exponent<1>;
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
template<typename BasicUnit>
struct find_first_exponent<BasicUnit,unsorted_unit_list<>> {
	using type = exponent<0>;
};
template<typename BasicUnit, typename Head, typename ...Tail>
struct find_first_exponent<BasicUnit,unsorted_unit_list<Head,Tail...>> {
	using type = typename std::conditional< get_exponent<Head,BasicUnit>::type::num != 0
	                                      , typename get_exponent<Head,BasicUnit>::type
	                                      , typename find_first_exponent<BasicUnit, unsorted_unit_list<Tail...>>::type >::type;
};
/** @} */

/** create an unsorted compile-time list of basic units from a unit */
template<typename Unit>
struct unsorted_unit_list_from_unit;
template<typename E1, typename E2, typename E3, typename E4, typename E5, typename E6, typename E7>
struct unsorted_unit_list_from_unit<unit<E1,E2,E3,E4,E5,E6,E7>> {
	using type = unsorted_unit_list< unit< E1, e0, e0, e0, e0, e0, e0 >
	                               , unit< e0, E2, e0, e0, e0, e0, e0 >
	                               , unit< e0, e0, E3, e0, e0, e0, e0 >
	                               , unit< e0, e0, e0, E4, e0, e0, e0 >
	                               , unit< e0, e0, e0, e0, E5, e0, e0 >
	                               , unit< e0, e0, e0, e0, e0, E6, e0 >
	                               , unit< e0, e0, e0, e0, e0, e0, E7 > >;
};

/** create a unit with the basic units being sorted into the correct order from a unsorted_unit_list */
template< typename UnsortedUnitList >
using unit_from_unsorted_list_t = unit< typename find_first_exponent<            time, UnsortedUnitList>::type
                                      , typename find_first_exponent<            mass, UnsortedUnitList>::type
                                      , typename find_first_exponent<          length, UnsortedUnitList>::type
                                      , typename find_first_exponent<         current, UnsortedUnitList>::type
                                      , typename find_first_exponent<      luminosity, UnsortedUnitList>::type
                                      , typename find_first_exponent<     temperature, UnsortedUnitList>::type
                                      , typename find_first_exponent<substance_amount, UnsortedUnitList>::type >;

/**
 * @{
 * meta functions for unit type manipulations
 */
template<template<typename> class Operation, typename Unit>
using apply_unary_t = unit< Operation<            time_exponent_t<Unit>>
                          , Operation<            mass_exponent_t<Unit>>
                          , Operation<          length_exponent_t<Unit>>
                          , Operation<         current_exponent_t<Unit>>
                          , Operation<      luminosity_exponent_t<Unit>>
                          , Operation<     temperature_exponent_t<Unit>>
                          , Operation<substance_amount_exponent_t<Unit>> >;

template<template<typename,typename> class Operation, typename Unit1, typename Unit2>
using apply_binary_t = unit< Operation<            time_exponent_t<Unit1>,             time_exponent_t<Unit2>>
                           , Operation<            mass_exponent_t<Unit1>,             mass_exponent_t<Unit2>>
                           , Operation<          length_exponent_t<Unit1>,           length_exponent_t<Unit2>>
                           , Operation<         current_exponent_t<Unit1>,          current_exponent_t<Unit2>>
                           , Operation<      luminosity_exponent_t<Unit1>,       luminosity_exponent_t<Unit2>>
                           , Operation<     temperature_exponent_t<Unit1>,      temperature_exponent_t<Unit2>>
                           , Operation<substance_amount_exponent_t<Unit1>, substance_amount_exponent_t<Unit2>> >;

template<typename E>
using   negate_exp_t = exponent<-E::num,E::den>;

template<int Number, bool = (Number<0) > struct sign;
template<int Number>                     struct sign<Number,false> { static const int value = +1; };
template<int Number>                     struct sign<Number,true > { static const int value = -1; };
template<>                               struct sign<0     ,false> { static const int value =  0; };

template<typename Unit, int Power, int Sign = sign<Power>::value> struct pow_unit;
template<typename Unit, int Power> struct pow_unit<Unit, Power,   +1> { using type = apply_binary_t<std::ratio_add     , Unit, typename pow_unit<Unit, Power-1>::type>; };
template<typename Unit, int Power> struct pow_unit<Unit, Power,   -1> { using type = apply_binary_t<std::ratio_subtract, Unit, typename pow_unit<Unit,-Power+1>::type>; };
template<typename Unit, int Sign > struct pow_unit<Unit,     0, Sign> { using type = unit<e0, e0, e0, e0, e0, e0, e0>; };

using cube_unit = unit<exponent<2>, exponent<2>, exponent<2>, exponent<2>, exponent<2>, exponent<2>, exponent<2>>;
/** @} */

}

/** a dimension-less unit */
using dimensionless = unit< exponent<0>
                          , exponent<0>
                          , exponent<0>
                          , exponent<0>
                          , exponent<0>
                          , exponent<0>
                          , exponent<0> >;

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
using create_unit_t = detail::unit_from_unsorted_list_t<detail::unsorted_unit_list<Basic1,Basic2,Basic3,Basic4,Basic5,Basic6,Basic7>>;

/**
 * @{
 *
 * @brief unit type manipulations
 */
template<typename Unit1, typename Unit2> using        mul_unit_t = detail::apply_binary_t<std::ratio_add     ,Unit1,Unit2>;
template<typename Unit1, typename Unit2> using        div_unit_t = detail::apply_binary_t<std::ratio_subtract,Unit1,Unit2>;
template<typename Unit , int Power>      using        pow_unit_t = typename detail::pow_unit<Unit,Power>::type;
template<typename Unit>                  using     square_unit_t = pow_unit_t<Unit,2>;
template<typename Unit>                  using       cube_unit_t = pow_unit_t<Unit,3>;
template<typename Unit>                  using reciprocal_unit_t = detail::apply_unary_t<detail::negate_exp_t,Unit>;
template<typename Unit>                  using       sqrt_unit_t = detail::apply_binary_t<std::ratio_divide, Unit, detail::cube_unit>;
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
                                                   , std::ratio_equal<typename Unit1::            time_exponent, typename Unit2::            time_exponent>::value
                                                  && std::ratio_equal<typename Unit1::            mass_exponent, typename Unit2::            mass_exponent>::value
                                                  && std::ratio_equal<typename Unit1::          length_exponent, typename Unit2::          length_exponent>::value
                                                  && std::ratio_equal<typename Unit1::         current_exponent, typename Unit2::         current_exponent>::value
                                                  && std::ratio_equal<typename Unit1::      luminosity_exponent, typename Unit2::      luminosity_exponent>::value
                                                  && std::ratio_equal<typename Unit1::     temperature_exponent, typename Unit2::     temperature_exponent>::value
                                                  && std::ratio_equal<typename Unit1::substance_amount_exponent, typename Unit2::substance_amount_exponent>::value>;

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
using unit_is_dimensionless = std::is_same<Unit,dimensionless>;

template<typename Unit>
constexpr bool unit_is_dimensionless_v = unit_is_dimensionless<Unit>::value;
/** @} */

}

#endif /* UNLIB_UNIT_HPP */
