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

#include <unlib/ratio.hpp>

namespace unlib {

/**
 * @brief Quantity tag
 *
 * Different kind of quantities of the same physical unit can be tagged in
 * order to make them incompatible, These tags consists of an ID and an
 * exponent. The ID is used to tell tags, tagged units and, ultimately, tagged
 * quantities, from each other. Any type, even an incomplete type, will do for
 * the ID. The exponent is used to track multiplication and division of tagged
 * quantities so that tags can be canceled automatically when dividing
 * quantities.
 *
 * @tparam       ID  tag ID
 * @tparam Exponent  tag exponent
 *
 * @note Do not instantiate this template  directly. Use the tag_t<> meta
 *       function instead.
 */
template<typename ID, typename Exponent>
struct tag;

/**
 * @brief Tag ratio
 *
 * Ratios are used to track multiplication and division of tagged quantities
 * so that tags can be canceled automatically when dividing tags.
 */
template< std::intmax_t Num = 1
        , std::intmax_t Den = 1 >
using tag_ratio_t = ratio_t<Num,Den>;

template<typename ID, std::intmax_t Num, std::intmax_t Den>
struct tag<ID,std::ratio<Num,Den>> {
	using id        = ID;
	using is_no_tag = std::integral_constant<bool, detail::is_same_v<id,void> or Num==0>;
	using exponent  = std::conditional_t< is_no_tag::value, tag_ratio_t<0,1>, typename tag_ratio_t<Num,Den>::type >;
	using type      = tag<std::conditional_t<is_no_tag::value || Num==0, void, ID>, exponent>;
};

/**
 * @brief No tag
 *
 * This signifies that a quantity has no tag
 */
using no_tag = tag<void,tag_ratio_t<0>>;

/**
 * @brief Create tag types
 *
 * This is a meta function to create tag types. The result will already be
 * cancelled, if possible. If either ID is void or Num is zero, this will
 * result in no_tag.
 *
 * @see tag
 */
template<typename ID, std::intmax_t Num = 1, std::intmax_t Den = 1>
using tag_t = typename tag<ID,tag_ratio_t<Num,Den>>::type;

namespace detail {

template<typename Tag1, typename Tag2>
struct add_tag_exponents;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct add_tag_exponents<tag<ID,std::ratio<N1,D1>>,tag<ID,std::ratio<N2,D2>>> {
	using type = tag_t< ID, std::ratio_add<tag_ratio_t<N1,D1>,tag_ratio_t<N2,D2>>::num
	                      , std::ratio_add<tag_ratio_t<N1,D1>,tag_ratio_t<N2,D2>>::den >;
};

template<typename Tag1, typename Tag2>
struct substract_tag_exponents;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct substract_tag_exponents<tag<ID,std::ratio<N1,D1>>,tag<ID,std::ratio<N2,D2>>> {
	using type = tag_t<ID, std::ratio_subtract<tag_ratio_t<N1,D1>,tag_ratio_t<N2,D2>>::num
	                     , std::ratio_subtract<tag_ratio_t<N1,D1>,tag_ratio_t<N2,D2>>::den >;
};


template<typename Tag1, typename Tag2>
struct mul_tag;
template<>
struct mul_tag<no_tag,no_tag>                                             {using type = no_tag;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct mul_tag<no_tag,tag<ID,std::ratio<N,D>>>                            {using type = tag_t<ID,N,D>;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct mul_tag<tag<ID,std::ratio<N,D>>,no_tag>                            {using type = tag_t<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct mul_tag<tag<ID,std::ratio<N1,D1>>,tag<ID,std::ratio<N2,D2>>>       {using type = typename add_tag_exponents<tag_t<ID,N1,D1>,tag_t<ID,N2,D2>>::type;};


template<typename Tag1, typename Tag2>
struct div_tag;
template<>
struct div_tag<no_tag,no_tag>                                             {using type = no_tag;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct div_tag<no_tag,tag<ID,std::ratio<N,D>>>                            {using type = tag_t<ID,N,D>;};
template<typename ID, std::intmax_t N , std::intmax_t D >
struct div_tag<tag<ID,std::ratio<N,D>>,no_tag>                            {using type = tag_t<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct div_tag<tag<ID,std::ratio<N1,D1>>,tag<ID,std::ratio<N2,D2>>>       {using type = typename substract_tag_exponents<tag_t<ID,N1,D1>,tag_t<ID,N2,D2>>::type;};


template<typename Tag1, typename Tag2>
struct are_tags_compatible;
template< typename ID1, std::intmax_t N1, std::intmax_t D1
        , typename ID2, std::intmax_t N2, std::intmax_t D2 >
struct are_tags_compatible<tag<ID1,std::ratio<N1,D1>>,tag<ID2,std::ratio<N2,D2>>>
	: std::integral_constant<bool, tag<ID1,tag_ratio_t<N1,D1>>::is_no_tag::value
	                            or tag<ID2,tag_ratio_t<N2,D2>>::is_no_tag::value
	                            or std::is_same<ID1,ID2>::value > {};

}

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
using are_tags_compatible = detail::are_tags_compatible<Tag1, Tag2>;
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
using mul_tag_t = typename detail:: mul_tag<Tag1, Tag2>::type;
template<typename Tag1, typename Tag2>
using div_tag_t = typename detail:: div_tag<Tag1, Tag2>::type;
/** @} */

/**
 * @{
 *
 * @brief Get tag resulting from quantity sqrt/pow operations
 *
 * @note If the parameter is a tag IDs, rather than a tag, a tag will be
 *       created from it before the operation.
 */
template<typename Tag, typename Ratio>
using pow_tag_t = typename tag<typename Tag::id, std::ratio_multiply<typename Tag::exponent,Ratio>>::type;
template<typename Tag>
using sqrt_tag_t = pow_tag_t<Tag, std::ratio<1,2>>;
/** @} */

}

#endif //UNLIB_TAG_HPP
