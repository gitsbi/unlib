#ifndef UNLIB_RATIO_HPP
#define UNLIB_RATIO_HPP

/*
 * exponent.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

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
using ratio_t = typename std::ratio<Num,Den>::type;

namespace detail {

template<int Number, bool = (Number<0) > struct sign;
template<int Number>                     struct sign<Number,false> { static const int value = +1; };
template<int Number>                     struct sign<Number,true > { static const int value = -1; };
template<>                               struct sign<0     ,false> { static const int value =  0; };

}

template<typename Exp1, typename Exp2>
constexpr bool ratio_equal_v = std::ratio_equal<Exp1,Exp2>::value;

template<typename Ratio>
struct ratio_negate {using type = ratio_t<-Ratio::num,Ratio::den>;};
template<typename Ratio>
using ratio_negate_t = typename ratio_negate<Ratio>::type;

template<typename Ratio>
struct ratio_reciprocal {using type = ratio_t<Ratio::den,Ratio::num>;};
template<typename Ratio>
using ratio_reciprocal_t = typename ratio_reciprocal<Ratio>::type;

template<int Number>
using sign = detail::sign<Number>;
template<int Number>
constexpr int sign_v = sign<Number>::value;

}

#endif //UNLIB_RATIO_HPP
