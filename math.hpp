#ifndef MGC_MATH_HPP
#define MGC_MATH_HPP

#include <cmath>
#include <unlib/quantity.hpp>

namespace unlib {

template<typename U, typename S, typename V, typename T>
auto abs(quantity<U,S,T,V> q) {
	return q < quantity<U,S,T,V>{0} ? -q : q;
}

template<int Power, typename U, typename S, typename V, typename T>
auto pow(quantity<U,S,T,V> q) {
	return pow_quantity_t<quantity<U,S,T,V>,Power>{std::pow(q.get(),Power)};
}

template<typename U, typename S, typename V, typename T>
auto sqrt(quantity<U,S,T,V> q) {
	return sqrt_quantity_t<quantity<U,S,T,V>>{std::sqrt(q.get())};
}

}

#endif //MGC_MATH_HPP
