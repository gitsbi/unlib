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
auto abs(quantity<U,S,T,V> q) {
	return q < quantity<U,S,T,V>{0} ? -q : q;
}

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
 * @return @p q, raised to Power
 */
template<typename Power, typename U, typename S, typename V, typename T>
auto pow(const quantity<U,S,T,V>& q) {
	return pow_quantity_t<quantity<U,S,T,V>, Power>{ std::pow( static_cast<double>(q.get())
	                                                         , static_cast<double>(Power::num)/Power::den ) };
}

template<int Power, typename U, typename S, typename V, typename T>
auto pow(quantity<U,S,T,V> q) {
	return pow<ratio_t<Power>>(q);
}
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
 * @note This operation will affect @p U, @p S, and @p T as well. If the type
 *       of @p q is not the result of a quantity raised to the power of two,
 *       then the resulting scale will be an awkward approximation of the
 *       correct value.
 */
template<typename U, typename S, typename V, typename T>
auto sqrt(quantity<U,S,T,V> q) {
	return pow<std::ratio<1,2>>(q);
}
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
 * @note The square root is only defined for selected values of @p S and @p T.
 *       @see ratio_root_t for more details.
 *
 * @note This operation will affect @p U, @p S, and @p T as well. If the type
 *       of @p q is not the result of a quantity raised to the power of three,
 *       then the resulting scale will be an awkward approximation of the
 *       correct value.
 */
template<typename U, typename S, typename V, typename T>
auto cbrt(quantity<U,S,T,V> q) {
	return pow<std::ratio<1,3>>(q);
}
/** @} */

}

#endif //UNLIB_MATH_HPP
