#ifndef MGC_MATH_HPP
#define MGC_MATH_HPP

#include <cmath>
#include <unlib/quantity.hpp>

namespace unlib {

template<typename U, typename S, typename V, typename T>
quantity<U,S,T,V> abs(quantity<U,S,T,V> q) {
	return q < quantity<U,S,T,V>{0} ? -q : q;
}

template<typename U, typename S, typename V, typename T>
auto sqrt(quantity<U,S,T,V> q) {
	return sqrt_quantity_t<quantity<U,S,T,V>>{std::sqrt(q.get())};
}

}

#endif //MGC_MATH_HPP
