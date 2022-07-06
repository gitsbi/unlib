#ifndef UNLIB_MATH_HPP
#define UNLIB_MATH_HPP

/*
 * math.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cmath>

#include <unlib/ratio.hpp>
#include <unlib/quantity.hpp>

namespace unlib {

namespace detail {

template<typename Float1, typename T>
using if_float1_pt_t = typename std::enable_if< is_floating_point<Float1>::value, T >::type;
template<typename Float1, typename Float2, typename T>
using if_float2_pt_t = typename std::enable_if< is_floating_point<Float1>::value
                                            and is_floating_point<Float2>::value, T >::type;

template<typename D, typename S, typename V, typename T, std::intmax_t Nom, std::intmax_t Den>
auto pow(const quantity<D,S,T,V>& q, const std::ratio<Nom,Den>) {
	return pow_quantity_t<quantity<D,S,T,V>, std::ratio<Nom,Den>>{ std::pow( static_cast<double>(q.get()), static_cast<double>(Nom)/Den ) };
}

template<typename U, typename S, typename V, typename T>
auto pow(const quantity<U,S,T,V>& q, const std::ratio<1,2>) {
	return pow_quantity_t<quantity<U,S,T,V>,std::ratio<1,2>>(std::sqrt(q.get()));
}

template<typename TolTag, typename F, typename>
struct tolerance_aux {
	using tolerance_tag_type = TolTag;
	using value_type = F;
	value_type v;
};

struct tolerance_val_tag;
struct tolerance_nom_tag;
struct tolerance_frc_tag;

template<typename V, typename S=scale_t<1>>
using fraction = quantity<dimensionless, S, V, no_tag>;

}

/**
 * @brief Absolute value
 *
 * This returnes the absolute value of a quantity
 *
 * @tparam U  Quantity's unit
 * @tparam S  Quantity's scale
 * @tparam V  Quantity's value type
 * @tparam T  Quantity's tag
 *
 * @param q  Quantity to get absolute value from
 *
 * @return absolute value of @p q
 */
template<typename U, typename S, typename V, typename T>
auto abs(quantity<U,S,T,V> q)                                             {return quantity<U,S,T,V>{std::abs(q.get())};}

/**
 * @{
 *
 * @brief Calculate the power of a base
 *
 * @tparam Power  Power to raise @p q to; must be integer or std::ratio
 * @tparam     U  Quantity's unit
 * @tparam     S  Quantity's scale
 * @tparam     V  Quantity's value type
 * @tparam     T  Quantity's tag
 *
 * @param q  Quantity to raise
 *
 * @return @p q, raised to Power, as pow_quantity_t<quantity<U,S,T,V>,
 */
template<typename Power, typename U, typename S, typename V, typename T>
auto pow(const quantity<U,S,T,V>& q)                                      {return detail::pow(q, typename Power::type{});}

template<int Power, typename U, typename S, typename V, typename T>
auto pow(quantity<U,S,T,V> q)                                             {return pow<ratio_t<Power>>(q);}
/** @} */

/**
 * @brief Get square root of a quantity
 *
 * @tparam U  Quantity's unit
 * @tparam S  Quantity's scale
 * @tparam V  Quantity's value type
 * @tparam T  Quantity's tag
 *
 * @param q  Quantity to get square root from
 *
 * @return square root of @p q
 *
 * @note The square root is only defined for selected values of @p S and @p T.
 *       @see ratio_root_t for more details.
 *
 * @note This operation will affect @p U, @p S, and @p T as well.
 */
template<typename U, typename S, typename V, typename T>
auto sqrt(quantity<U,S,T,V> q)                                            {return pow_quantity_t<quantity<U,S,T,V>,std::ratio<1,2>>(std::sqrt(q.get()));}
/** @} */

/**
 * @brief Get cube root of a quantity
 *
 * @tparam U  Quantity's unit
 * @tparam S  Quantity's scale
 * @tparam V  Quantity's value type
 * @tparam T  Quantity's tag
 *
 * @param q  Quantity to get cube root from
 *
 * @return cube root of @p q
 *
 * @note The cube root is only defined for selected values of @p S and @p T.
 *       @see ratio_root_t for more details.
 *
 * @note This operation will affect @p U, @p S, and @p T as well.
 */
template<typename U, typename S, typename V, typename T>
auto cbrt(quantity<U,S,T,V> q)                                            {return pow<std::ratio<1,3>>(q);}
/** @} */

/**
 * @brief Provide the minimum of two quantities
 *
 * @param lhs, rhs  quantities to compare
 *
 * @return minimum value of @p lhs and @p rhs
 */
template<typename D, typename F, typename S1, typename S2, typename T>
auto min(const unlib::quantity<D,S1,F,T> lhs, const unlib::quantity<D,S2,F,T> rhs) {
	using std::min;
	return unlib::quantity<D,S1,F,T>{min(lhs.get(), rhs.template get_scaled<S1>())};
}

/**
 * @brief Provide the maximum of two quantities
 *
 * @param lhs, rhs  quantities to compare
 *
 * @return maximum value of @p lhs and @p rhs
 */
template<typename D, typename F, typename S1, typename S2, typename T>
auto max(const unlib::quantity<D,S1,F,T> lhs, const unlib::quantity<D,S2,F,T> rhs) {
	using std::max;
	return unlib::quantity<D,S1,F,T>{max(lhs.get(), rhs.template get_scaled<S1>())};
}

/**
 * @{
 *
 * @brief Floating point comparisons
 *
 * Due to the limitations of floating point precision, more often than
 * never values which, mathematically, ought to be equal, are in fact not
 * totally equal. Therefore, floating point comparisons should be done with
 * a certain tolerance.
 *
 * This provides a framework for comparing floating point types and
 * quantities of floating point types. The tolerance can either be an
 * absolute value, or a fraction of a nominal value. For example, in order to
 * test whether two masses are equal, the tolerance could be given either as
 * an absolute value (10mg) or as a fraction (0.1%) of a nominal weight (1t).
 * The library provides default values for the nominal and the weight, which
 * can be used, but this must be indicated explicitly.
 *
 * The defaults are accessed via traits which can be specialized by users in
 * order to provide their own defaults.
 */

constexpr double tolerance_default_nominal  = 1.;     /**< default tolerance nominal for is_near() etc. */
constexpr double tolerance_default_fraction = 0.0001; /**< default tolerance fraction for is_near() etc. */

/**
 * @{
 *
 * @brief wrapper for tolerance values
 *
 * In order to distinguish between tolerance values, nominals, and fractions
 * in floating point comparisons, those are wrapped in these types.
 *
 * @note These are the result types of @ref tolerance_value(), @ref tolerance_nominal(),
 *       and @ref tolerance_fraction(). Users should not have to deal with them
 *       directly.
 */
template<typename V>             using tolerance_val = detail::tolerance_aux<detail::tolerance_val_tag,V,V>;
template<typename V>             using tolerance_nom = detail::tolerance_aux<detail::tolerance_nom_tag,V,V>;
template<typename V, typename Q> using tolerance_frc = detail::tolerance_aux<detail::tolerance_frc_tag,V,Q>;
/** @} */

/**
 * @{
 *
 * @brief tolerance value calculations
 *
 * A tolerance value can be calculated by multiplying a nominal and a fraction.
 *
 * @param n  tolerance nominal value
 * @param f  tolerance fraction value
 *
 * @return tolerance value
 */
template<typename U, typename S, typename SF, typename V, typename T, typename Q>
inline constexpr
tolerance_val<quantity<U,S,V,T>> operator*(tolerance_nom<quantity<U,S,V,T>> n, tolerance_frc<detail::fraction<V,SF>,Q> f)
{return tolerance_val<quantity<U,S,V,T>>{n.v * f.v.template get_scaled<no_scaling>()};}

template<typename F, typename SF, typename Q>
inline constexpr
tolerance_val<F> operator*(tolerance_nom<F> n, tolerance_frc<detail::fraction<F,SF>,Q> f)
{return tolerance_val<F>{n.v * f.v.template get_scaled<no_scaling>()};}
/** @} */

/**
 * @{
 * @brief tolerance traits
 *
 * These provide the necessary functions to obtain default tolerance values,
 * nominals, and fractions.
 *
 * @tparam F  Floating point type.
 */
template<typename F>
struct tolerance_traits {
	using float_t = detail::if_float1_pt_t<F,F>;
	using fract_t = detail::fraction<float_t>;

	static constexpr auto value   ()                  {return nominal() * fraction();}
	static constexpr auto nominal ()                  {return tolerance_nom<float_t        >{        static_cast<float_t>(tolerance_default_nominal ) };}
	static constexpr auto fraction()                  {return tolerance_frc<fract_t,float_t>{fract_t{static_cast<float_t>(tolerance_default_fraction)}};}
};

/** specialization for quantities of floating point types */
template<typename U, typename S, typename F, typename T>
struct tolerance_traits<quantity<U,S,F,T>> {
	using float_t = detail::if_float1_pt_t<F,F>;
	using quant_t = quantity<U,S,float_t,T>;
	using fract_t = detail::fraction<float_t>;

	static constexpr auto value   ()                  {return nominal() * fraction();}
	static constexpr auto nominal ()                  {return tolerance_nom<quant_t        >{quant_t{tolerance_traits<float_t>::nominal ().v}};}
	static constexpr auto fraction()                  {return tolerance_frc<fract_t,quant_t>{        tolerance_traits<float_t>::fraction().v };}
};
/** @} */

/**
 * @{
 *
 * @brief create a tolerance value, nominal, or fraction
 *
 * These function turn a value into a tolerance value, nominal, or fraction.
 *
 * @param val  value to turn into a tolerance value, nominal, or fraction
 *
 * @return tolerance value, nominal, or fraction
 */
template<typename T>             constexpr auto tolerance_value   (T val) {return tolerance_val<T  >{val};}
template<typename T>             constexpr auto tolerance_nominal (T nom) {return tolerance_nom<T  >{nom};}
template<typename T, typename F> constexpr auto tolerance_fraction(F frc) {return tolerance_frc<F,T>{frc};}
/** @} */

/**
 * @{
 *
 * @brief get the default tolerance value, nominal, or fraction
 *
 * These function return the tolerance value, nominal, or fraction for a
 * given type.
 *
 * @return default tolerance value, nominal, or fraction
 */
template<typename T>             constexpr auto tolerance_value   ()      {return tolerance_traits<T>::value   ();}
template<typename T>             constexpr auto tolerance_nominal ()      {return tolerance_traits<T>::nominal ();}
template<typename T>             constexpr auto tolerance_fraction()      {return tolerance_traits<T>::fraction();}
/** @} */

namespace detail {

template<typename V, typename F, typename Q> constexpr auto get_tol_val(tolerance_nom<V>   n, tolerance_frc<F,Q> f) {return n * f;}
template<typename V, typename F, typename Q> constexpr auto get_tol_val(tolerance_frc<F,Q> f, tolerance_nom<V>   n) {return get_tol_val(n,f);}
template<typename V>                         constexpr auto get_tol_val(tolerance_aux<tolerance_val_tag,V,V>     v) {return v;}
template<typename V>                         constexpr auto get_tol_val(tolerance_aux<tolerance_nom_tag,V,V>     n) {return get_tol_val(n, tolerance_traits<V>::fraction());}
template<typename F, typename Q>             constexpr auto get_tol_val(tolerance_aux<tolerance_frc_tag,F,Q>     f) {return get_tol_val(f, tolerance_traits<Q>::nominal ());}

template<typename T1, typename T2, typename Tol> constexpr bool is_near_impl   (T1 lval, T2 rval, tolerance_val<Tol> tol) {using std::abs; return abs(lval-rval) <= abs(tol.v);}
template<typename T1, typename T2, typename Tol> constexpr bool is_smaller_impl(T1 lval, T2 rval, tolerance_val<Tol> tol) {using std::abs; return not is_near_impl(lval,rval,tol) and lval<rval;}
template<typename T1, typename T2, typename Tol> constexpr bool is_greater_impl(T1 lval, T2 rval, tolerance_val<Tol> tol) {using std::abs; return not is_near_impl(lval,rval,tol) and lval>rval;}

template<typename T1, typename T2, typename Aux>                 constexpr detail::if_float2_pt_t<T1,T2,bool> is_near   (T1 lval, T2 rval, Aux  tol)             {return is_near_impl   (lval, rval, get_tol_val(tol));}
template<typename T1, typename T2, typename Aux>                 constexpr detail::if_float2_pt_t<T1,T2,bool> is_smaller(T1 lval, T2 rval, Aux  tol)             {return is_smaller_impl(lval, rval, get_tol_val(tol));}
template<typename T1, typename T2, typename Aux>                 constexpr detail::if_float2_pt_t<T1,T2,bool> is_greater(T1 lval, T2 rval, Aux  tol)             {return is_greater_impl(lval, rval, get_tol_val(tol));}
template<typename T1, typename T2, typename Aux1, typename Aux2> constexpr detail::if_float2_pt_t<T1,T2,bool> is_near   (T1 lval, T2 rval, Aux1 tol1, Aux2 tol2) {return is_near_impl   (lval, rval, get_tol_val(tol1,tol2));}
template<typename T1, typename T2, typename Aux1, typename Aux2> constexpr detail::if_float2_pt_t<T1,T2,bool> is_smaller(T1 lval, T2 rval, Aux1 tol1, Aux2 tol2) {return is_smaller_impl(lval, rval, get_tol_val(tol1,tol2));}
template<typename T1, typename T2, typename Aux1, typename Aux2> constexpr detail::if_float2_pt_t<T1,T2,bool> is_greater(T1 lval, T2 rval, Aux1 tol1, Aux2 tol2) {return is_greater_impl(lval, rval, get_tol_val(tol1,tol2));}

}

/** @{
 * @note These algorithms take tolerances. As tolerances, either an absolute
 *       tolerance value (created via @ref tolerance_value()) or a tolerance
 *       nominal (created via @ref tolerance_nominal()) and a tolerance
 *       fraction (created via @ref tolerance_fraction()) can be passed.
 *       Either the nominal or the fraction can also be omitted, in which
 *       case the default is obtained via the @ref tolerance_traits.
 *
 * @note Tolerances are inclusive. That is, two values which differ exactly by
 *       the tolerance value are considered equal.
 *
 * @note These algorithms will always use the `abs()` value of the tolerance,
 *       so the sign of the tolerance values will be ignored.
 *
 */

/** @{
 * @brief compare two floating point values
 *
 * This compares two floating point values or two quantities of floating point
 * values.
 *
 * @param lval              left value to compare
 * @param rval              right value to compare
 * @param tol, tol1, tol2   tolerance
 *
 * @note The quantities do not need to be of the same scale, but they need to
 *       have the same unit, value type, and tag.
 */
/**
 * @{
 * @return true if both quantities are equal within the given tolerance.
 */
template<typename F1, typename F2, typename TT, typename TF, typename X>
bool is_near   (F1 lval, F2 rval, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_near   (lval, rval, tol);}
template<typename F1, typename F2, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_near   (F1 lval, F2 rval, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_near   (lval, rval, tol1, tol2);}
/** @} */
/**
 * @{
 * @return true if both quantities are not equal within the given tolerance
 *         and the left value is smaller than the right value.
 */
template<typename F1, typename F2, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_smaller(F1 lval, F2 rval, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_smaller(lval, rval, tol1, tol2);}
template<typename F1, typename F2, typename TT, typename TF, typename X>
bool is_smaller(F1 lval, F2 rval, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_smaller(lval, rval, tol);}
/** @} */
/**
 * @{
 * @return true if both quantities are not equal within the given tolerance
 *         and the left value is greater than the right value.
 */
template<typename F1, typename F2, typename TT, typename TF, typename X>
bool is_greater(F1 lval, F2 rval, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_greater(lval, rval, tol);}
template<typename F1, typename F2, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_greater(F1 lval, F2 rval, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_greater(lval, rval, tol1, tol2);}
/** @} */
/** @} */

/** @{
 * @brief compare a floating point value to zero
 *
 * This compares a floating point value, or a quantity of a floating point
 * value, with zero.
 *
 * @param val               value to compare
 * @param tol, tol1, tol2   tolerance
 *
 */
/* @{
 *  @return true if the quantity is equal to zero within the tolerance
 *          given
 */
template<typename F, typename TT, typename TF, typename X>
bool is_near_zero   (F val, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_near   (val,F{}, tol);}
template<typename F, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_near_zero   (F val, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_near   (val,F{}, tol1, tol2);}
/** @} */
/* @{
 *  @return true if the quantity is smaller than zero within the tolerance
 *          given
 */
template<typename F, typename TT, typename TF, typename X>
bool is_smaller_zero(F val, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_smaller(val,F{}, tol);}
template<typename F, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_smaller_zero(F val, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_smaller(val,F{}, tol1, tol2);}
/** @} */
/* @{
 *  @return true if the quantity is greater than zero within the tolerance
 *          given
 */
template<typename F, typename TT, typename TF, typename X>
bool is_greater_zero(F val, detail::tolerance_aux<TT,TF,X> tol) {return detail::is_greater(val,F{}, tol);}
template<typename F, typename TT1, typename TF1, typename TT2, typename TF2, typename X1, typename X2>
bool is_greater_zero(F val, detail::tolerance_aux<TT1,TF1,X1> tol1, detail::tolerance_aux<TT2,TF2,X2> tol2) {return detail::is_greater(val,F{}, tol1, tol2);}
/** @} */
/** @} */

/** @} */

/** @} */

}

#endif //UNLIB_MATH_HPP
