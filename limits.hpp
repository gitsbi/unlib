#ifndef UNLIB_LIMITS_HPP
#define UNLIB_LIMITS_HPP

/*
 * limits.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <limits>

#include <unlib/quantity.hpp>

namespace std {

template<typename U, typename S, typename V, typename T>
struct numeric_limits<unlib::quantity<U,S,V,T>> : public std::numeric_limits<V> {
	static constexpr  unlib::quantity<U,S,V,T> min          () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::min          ()};}
	static constexpr  unlib::quantity<U,S,V,T> max          () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::max          ()};}
	static constexpr  unlib::quantity<U,S,V,T> lowest       () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::lowest       ()};}
	static constexpr  unlib::quantity<U,S,V,T> epsilon      () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::epsilon      ()};}
	static constexpr  unlib::quantity<U,S,V,T> round_error  () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::round_error  ()};}
	static constexpr  unlib::quantity<U,S,V,T> infinity     () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::infinity     ()};}
	static constexpr  unlib::quantity<U,S,V,T> quiet_NaN    () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::quiet_NaN    ()};}
	static constexpr  unlib::quantity<U,S,V,T> signaling_NaN() noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::signaling_NaN()};}
	static constexpr  unlib::quantity<U,S,V,T> denorm_min   () noexcept   {return unlib::quantity<U,S,V,T>{std::numeric_limits<V>::denorm_min   ()};}
};

}

#endif //UNLIB_LIMITS_HPP
