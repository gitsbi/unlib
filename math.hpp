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
#include <unlib/quantity.hpp>

namespace unlib {

template<typename U, typename S, typename V, typename T>
auto abs(quantity<U,S,T,V> q)
{return q < quantity<U,S,T,V>{0} ? -q : q;}

template<int Power, typename U, typename S, typename V, typename T>
auto pow(quantity<U,S,T,V> q)
{return pow_quantity_t<quantity<U,S,T,V>,Power>{std::pow(q.get(),Power)};}

template<typename U, typename S, typename V, typename T>
auto sqrt(quantity<U,S,T,V> q)
{return sqrt_quantity_t<quantity<U,S,T,V>>{std::sqrt(q.get())};}

}

#endif //UNLIB_MATH_HPP
