#ifndef UNLIB_TAG_HPP
#define UNLIB_TAG_HPP

/*
 * tag.hpp
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
 * @brief Quantity tag
 *
 * Quantities can be tagged with a tag, which consists of an ID and an
 * exponent. The ID is used to tell tags from each other. Any type, even an
 * incomplete type, will do for the ID. The exponent is used to track
 * multiplication and division of tagged quantities so that tags can be
 * canceled automatically when divided.
 *
 * @tparam  ID  tag ID
 * @tparam Num  tag exponent numerator
 * @tparam Den  tag exponent denominator
 *
 * @note Do not this type directly. Use the create_tag_t meta function instead.
 */
template<typename ID, std::intmax_t Num, std::intmax_t Den>
struct tag {
	using id        = ID;
	using is_no_tag = std::is_same<id,void>;
	using exponent  = typename std::conditional<is_no_tag::value, std::ratio<0,1>, typename std::ratio<Num,Den>::type>::type;
};

/**
 * @brief No tag
 *
 * This signifies that a quantity has no tag
 */
using no_tag = tag<void,0,1>;

namespace detail {

template<typename ID, std::intmax_t N=0, std::intmax_t D=0>
struct create_tag                                               {using type = tag<ID,N,D>;};
template<             std::intmax_t N  , std::intmax_t D  >
struct create_tag<void,N,D>                                     {using type = no_tag;};
template<typename ID                   , std::intmax_t D  >
struct create_tag<ID,0,D>                                       {using type = no_tag;};
template<                                std::intmax_t D  >
struct create_tag<void,0,D>                                     {using type = no_tag;};
template<typename ID, std::intmax_t N  , std::intmax_t D  >
struct create_tag<tag<ID,N,D>,0,0>                              {using type = typename create_tag<ID,N ,D >::type;};
template<typename ID, std::intmax_t N1 , std::intmax_t D1
                    , std::intmax_t N2 , std::intmax_t D2 >
struct create_tag<tag<ID,N1,D1>,N2,D2>                          {using type = typename create_tag<ID,N1,D1>::type;};


template<typename Tag1, typename Tag2>
struct add_tag_exponents;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct add_tag_exponents<tag<ID,N1,D1>,tag<ID,N2,D2>> {
	using type = typename create_tag<tag<ID, std::ratio_add<std::ratio<N1,D1>,std::ratio<N2,D2>>::num
	                                       , std::ratio_add<std::ratio<N1,D1>,std::ratio<N2,D2>>::den>>::type;
};

template<typename Tag1, typename Tag2>
struct substract_tag_exponents;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct substract_tag_exponents<tag<ID,N1,D1>,tag<ID,N2,D2>> {
	using type = typename create_tag<tag<ID, std::ratio_subtract<std::ratio<N1,D1>,std::ratio<N2,D2>>::num
	                                       , std::ratio_subtract<std::ratio<N1,D1>,std::ratio<N2,D2>>::den>>::type;
};


template<typename Tag1, typename Tag2>
struct mul_tag;

template<>
struct mul_tag<no_tag,no_tag>                                   {using type = no_tag;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct mul_tag<no_tag,tag<ID,N,D>>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct mul_tag<tag<ID,N,D>,no_tag>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct mul_tag<tag<ID,N1,D1>,tag<ID,N2,D2>>                     {using type = typename add_tag_exponents<tag<ID,N1,D1>,tag<ID,N2,D2>>::type;};


template<typename Tag1, typename Tag2>
struct div_tag;

template<>
struct div_tag<no_tag,no_tag>                                   {using type = no_tag;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct div_tag<no_tag,tag<ID,N,D>>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct div_tag<tag<ID,N,D>,no_tag>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct div_tag<tag<ID,N1,D1>,tag<ID,N2,D2>>                     {using type = typename substract_tag_exponents<tag<ID,N1,D1>,tag<ID,N2,D2>>::type;};


template<typename Tag>
struct sqrt_tag : create_tag<typename Tag::id, std::ratio_divide<typename Tag::exponent,std::ratio<2>>::num
                                             , std::ratio_divide<typename Tag::exponent,std::ratio<2>>::den> {};


template<typename Tag, int Power>
struct  pow_tag : create_tag<typename Tag::id, std::ratio_multiply<typename Tag::exponent,std::ratio<Power>>::num
                                             , std::ratio_multiply<typename Tag::exponent,std::ratio<Power>>::den > {};


template<typename Tag1, typename Tag2>
struct are_tags_compatible;
template< typename ID1, std::intmax_t N1, std::intmax_t D1
        , typename ID2, std::intmax_t N2, std::intmax_t D2 >
struct are_tags_compatible<tag<ID1,N1,D1>,tag<ID2,N2,D2>> : std::integral_constant<bool, tag<ID1,N1,D1>::is_no_tag::value
                                                                                      or tag<ID2,N2,D2>::is_no_tag::value
                                                                                      or std::is_same<ID1,ID2>::value > {};

}

/**
 * @brief Tag creation
 *
 * This creates a tag from an ID and a exponent.
 *
 * @tparam  ID  tag ID
 * @tparam Num  tag exponent numerator
 * @tparam Den  tag exponent denominator
 *
 * @note While users can invoke this meta-function to create tags from their
 *       tag IDs, they can also just pass tag IDs, which the library will then
 *       turn into tags.
 *       In order for the library to do this, this meta-function can be
 *       invoked with either a tag ID (and, optionally, the tag exponent's
 *       numerator and denominator), or with a tag. If a tag is passed as
 *       the ID parameter, Num and Dem will be ignored and the tag returned
 *       instead. This allows the library to pass whatever users provide
 *       through this meta function and get a valid tag from it.
 *
 */
template<typename ID, std::intmax_t Num = 1, std::intmax_t Den = 1>
using create_tag_t = typename detail::create_tag<ID,Num,Den>::type;

/**
 * @{
 *
 * @brief Tag compatibility
 *
 * Check whether two tags are compatible for multiplication and division. Two
 * tags are compatible if they have the same ID or if either is the no_name
 * tag.
 *
 * @note If either or both parameters are tag IDs, rather than tags, tags will
 *       be created from them before checking them.
 *
 * @tparam Tag1  tag to check for compatibility
 * @tparam Tag2  tag to check for compatibility
 */
template<typename Tag1, typename Tag2>
using are_tags_compatible = detail::are_tags_compatible<create_tag_t<Tag1>, create_tag_t<Tag2>>;
template<typename Tag1, typename Tag2>
constexpr bool are_tags_compatible_v = are_tags_compatible<Tag1, Tag2>::value;
/** @} */


/**
 * @{
 *
 * @brief Get tag resulting from multiplying/dividing quantities
 *
 * This will calculate the tags resulting from multiplying/dividing quantities.
 *
 * The resulting tag is defined thus:
 *
 *  1. If both tags are the `no_tag`, then that is the resulting tag, too.
 *  2. If either of the tags is `no_tag`, the resulting tag will be the other
 *     tag.
 *  3. If both tags are equal, the resulting tag ID will be the same, their
 *     exponents added/subtracted.
 *  4. If both units have differing tags and neither is `no_tag`, the
 *     operation is not possible and will not compile.
 *
 * This means that, for example, you can multiply reactive power with time and
 * the result will be reactive energy. If you divide reactive energy by
 * reactive power, you get time, with the reactive tag removed.
 *
 * @note If either or both parameters are tag IDs, rather than tags, tags will
 *       be created from them before the operation.
 */
template<typename Tag1, typename Tag2>
using mul_tag_t = typename detail:: mul_tag<create_tag_t<Tag1>, create_tag_t<Tag2>>::type;
template<typename Tag1, typename Tag2>
using div_tag_t = typename detail:: div_tag<create_tag_t<Tag1>, create_tag_t<Tag2>>::type;
/** @} */

/**
 * @{
 *
 * @brief Get tag resulting from quantity sqrt/pow operations
 *
 * @note If the parameter is a tag IDs, rather than a tag, a tag will be
 *       created from it before the operation.
 */
template<typename Tag>
using sqrt_tag_t = typename detail::sqrt_tag<create_tag_t<Tag>>::type;
template<typename Tag, int Power>
using pow_tag_t = typename detail::  pow_tag<create_tag_t<Tag>, Power>::type;
/** @} */

}

#endif //UNLIB_TAG_HPP
