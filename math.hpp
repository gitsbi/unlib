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

#include <unlib/unlib/ratio.hpp>
#include <unlib/quantity.hpp>

namespace unlib {

namespace detail {

template<typename D, typename S, typename V, typename T, std::intmax_t Nom, std::intmax_t Den>
auto pow(const quantity<D,S,T,V>& q, const std::ratio<Nom,Den>) {
	return pow_quantity_t<quantity<D,S,T,V>, std::ratio<Nom,Den>>{ std::pow( static_cast<double>(q.get()), static_cast<double>(Nom)/Den ) };
}

template<typename D, typename S, typename V, typename T>
auto pow(const quantity<D,S,T,V>& q, const std::ratio<1,2>) {
	return pow_quantity_t<quantity<D,S,T,V>,std::ratio<1,2>>(std::sqrt(q.get()));
}

}

/**
 * @brief Absolute value
 *
 * This returnes the absolute value of a quantity
 *
 * @tparam D  Quantity's dimension
 * @tparam S  Quantity's scale
 * @tparam V  Quantity's value type
 * @tparam T  Quantity's tag
 *
 * @param q  Quantity to get absolute value from
 *
 * @return absolute value of @p q
 */
template<typename D, typename S, typename V, typename T>
auto abs(quantity<D,S,T,V> q)                                             {return quantity<D,S,T,V>{std::abs(q.get())};}

/**
 * @{
 *
 * @brief Calculate the power of a base
 *
 * @tparam Power  Power to raise @p q to; must be integer or std::ratio
 * @tparam     D  Quantity's dimension
 * @tparam     S  Quantity's scale
 * @tparam     V  Quantity's value type
 * @tparam     T  Quantity's tag
 *
 * @param q  Quantity to raise
 *
 * @return @p q, raised to Power, as pow_quantity_t<quantity<D,S,T,V>,
 */
template<typename Power, typename D, typename S, typename V, typename T>
auto pow(const quantity<D,S,T,V>& q)                                      {return detail::pow(q, typename Power::type{});}

template<int Power, typename D, typename S, typename V, typename T>
auto pow(quantity<D,S,T,V> q)                                             {return pow<ratio_t<Power>>(q);}
/** @} */

/**
 * @brief Get square root of a quantity
 *
 * @tparam D  Quantity's dimension
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
 * @note This operation will affect @p D, @p S, and @p T as well.
 */
template<typename D, typename S, typename V, typename T>
auto sqrt(quantity<D,S,T,V> q)                                            {return pow_quantity_t<quantity<D,S,T,V>,std::ratio<1,2>>(std::sqrt(q.get()));}
/** @} */

/**
 * @brief Get cube root of a quantity
 *
 * @tparam D  Quantity's dimension
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
 * @note This operation will affect @p D, @p S, and @p T as well.
 */
template<typename D, typename S, typename V, typename T>
auto cbrt(quantity<D,S,T,V> q)                                            {return pow<std::ratio<1,3>>(q);}
/** @} */

}

#endif //UNLIB_MATH_HPP
