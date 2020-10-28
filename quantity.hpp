#ifndef UNLIB_QUANTITY_HPP
#define UNLIB_QUANTITY_HPP

/*
 * quantity.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <unlib/unit.hpp>
#include <cstdint>
#include <ostream>
#include <istream>
#include <limits>
#include <ratio>
#include <type_traits>


namespace unlib {

namespace detail {
/* workaround for C++14 lacking this */
template<typename T1, typename T2>
constexpr bool is_same_v = std::is_same<T1,T1>::value;
}

/* scalings ******************************************************************/

/** represents a quantity's scale */
template< std::intmax_t Num
        , std::intmax_t Den = 1 >
using scale = std::ratio<Num,Den>;

/** an unscaled value */
using     no_scaling = scale<1>;

/**
 * @{
 * import std ratios so they are available in the same namespace as no_scaling is
 */
using   atto_scaling = std:: atto;
using  femto_scaling = std::femto;
using   pico_scaling = std:: pico;
using   nano_scaling = std:: nano;
using  micro_scaling = std::micro;
using  milli_scaling = std::milli;
using  centi_scaling = std::centi;
using   deci_scaling = std:: deci;
using   deca_scaling = std:: deca;
using  hecto_scaling = std::hecto;
using   kilo_scaling = std:: kilo;
using   mega_scaling = std:: mega;
using   giga_scaling = std:: giga;
using   tera_scaling = std:: tera;
using   peta_scaling = std:: peta;
using    exa_scaling = std::  exa;
/** @} */

/**
 * @{
 * humans measure time in odd multiples of seconds
 */
using second_scaling = no_scaling;
using minute_scaling = scale<60>;
using   hour_scaling = std::ratio_multiply<minute_scaling, scale<60>>;
using    day_scaling = std::ratio_multiply<  hour_scaling, scale<24>>;
using   week_scaling = std::ratio_multiply<   day_scaling, scale< 7>>;
/** @} */


/* tags **********************************************************************/

/**
 * @brief Quantity tag
 *
 * Tagged quantities are tagged with a tag, which consists of an ID and an
 * exponent. The ID is used to tell tags from each other. Any type, even an
 * incomplete type, will do for the ID. The exponent is used to track
 * multiplication and division of tagged quantities so that tags can be
 * canceled automatically when divided.
 *
 * @tparam  ID  tag ID
 * @tparam Num  tag ratio numerator
 * @tparam Den  tag ratio denominator
 *
 * @note Do not this type directly. Use the create_tag_t meta function instead.
 */
template<typename ID, std::intmax_t Num, std::intmax_t Den>
struct tag {
	using id        = ID;
	using is_no_tag = std::is_same<id,void>;
	using exponent  = typename std::conditional<is_no_tag::value, exponent<0,1>, exponent<Num,Den>>::type;
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
struct tag_add;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct tag_add<tag<ID,N1,D1>,tag<ID,N2,D2>> {
	using type = typename create_tag<tag<ID, std::ratio_add<std::ratio<N1,D1>,std::ratio<N2,D2>>::num
	                                       , std::ratio_add<std::ratio<N1,D1>,std::ratio<N2,D2>>::den>>::type;
};

template<typename Tag1, typename Tag2>
struct tag_subtract;
template<typename ID, std::intmax_t N1, std::intmax_t D1, std::intmax_t N2, std::intmax_t D2>
struct tag_subtract<tag<ID,N1,D1>,tag<ID,N2,D2>> {
	using type = typename create_tag<tag<ID, std::ratio_subtract<std::ratio<N1,D1>,std::ratio<N2,D2>>::num
	                                       , std::ratio_subtract<std::ratio<N1,D1>,std::ratio<N2,D2>>::den>>::type;
};


template<typename Tag1, typename Tag2>
struct mul_tag;

template<>
struct mul_tag<no_tag,no_tag>                                   {using type = no_tag;};
template<typename ID, std::intmax_t N, std::intmax_t D>
struct mul_tag<no_tag,tag<ID,N,D>>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N, std::intmax_t D>
struct mul_tag<tag<ID,N,D>,no_tag>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct mul_tag<tag<ID,N1,D1>,tag<ID,N2,D2>>                     {using type = typename tag_add<tag<ID,N1,D1>,tag<ID,N2,D2>>::type;};


template<typename Tag1, typename Tag2>
struct div_tag;

template<>
struct div_tag<no_tag,no_tag>                                   {using type = no_tag;};
template<typename ID, std::intmax_t N, std::intmax_t D>
struct div_tag<no_tag,tag<ID,N,D>>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N, std::intmax_t D>
struct div_tag<tag<ID,N,D>,no_tag>                              {using type = tag<ID,N,D>;};
template<typename ID, std::intmax_t N1, std::intmax_t D1
                    , std::intmax_t N2, std::intmax_t D2>
struct div_tag<tag<ID,N1,D1>,tag<ID,N2,D2>>                     {using type = typename tag_subtract<tag<ID,N1,D1>,tag<ID,N2,D2>>::type;};


template<typename Tag1, typename Tag2>
struct are_tags_compatible;
template< typename ID1, std::intmax_t N1, std::intmax_t D1
        , typename ID2, std::intmax_t N2, std::intmax_t D2 >
struct are_tags_compatible<tag<ID1,N1,D1>,tag<ID2,N2,D2>> : std::integral_constant<bool, tag<ID1,N1,D1>::is_no_tag::value
	                                                                                  or tag<ID2,N2,D2>::is_no_tag::value
	                                                                                  or detail::is_same_v<ID1,ID2> > {
};
template<typename Tag1, typename Tag2>
struct are_tags_compatible : are_tags_compatible< typename create_tag<Tag1>::type
	                                            , typename create_tag<Tag2>::type > {};

}

/**
 * @brief Tag creation
 *
 * This creates a tag from an ID and a ratio.
 *
 * @tparam  ID  tag ID
 * @tparam Num  tag ratio numerator
 * @tparam Den  tag ratio denominator
 *
 * @note If you pass a tag as the ID parameter, Num and Dem will be ignored
 *       and the tag returned instead. This allows this meta function to be
 *       called with either a tag ID or a tag and always return a valid tag,
 *       in turn allowing users to provide tag names only, rather than to
 *       create tags each time.
 */
template<typename ID, std::intmax_t Num = 1, std::intmax_t Den = 1>
using create_tag_t = typename detail::create_tag<ID,Num,Den>::type;

/**
 * @{
 *
 * @brief Tag compatibility
 *
 * Check whether two tags are compatible. If either or both parameters are tag
 * names, rather than tags, tags will be created from them before checking
 * them.
 *
 * @tparam Tag1  tag to check for compatibility
 * @tparam Tag2  tag to check for compatibility
 *
 * @note Two tags are compatible if they have the same ID or if either is
 *       the no_name tag.
 */
template<typename Tag1, typename Tag2>
using are_tags_compatible = detail::are_tags_compatible<Tag1, Tag2>;
template<typename Tag1, typename Tag2>
constexpr bool are_tags_compatible_v = are_tags_compatible<Tag1, Tag2>::value;
/** @} */


/**
 * @{
 *
 * @brief Get tag resulting from quantity multiplying/dividing
 *
 * This will calculate the tags resulting from multiplying/dividing quantities.
 *
 * The resulting tag is defined thus:
 *
 *  1. If both tags are the `no_tag`, then that is the resulting tag, too.
 *  2. If either of the tags is `no_tag`, the resulting tag will be the other
 *     tag.
 *  3. If both tags are equal, the resulting tag ID will be the same, their
 *     ratios added/subtracted.
 *  4. If both units have differing tags and neither is `no_tag`, the
 *     operation is not possible and will not compile.
 *
 * This means that, for example, you can multiply reactive power with time and
 * the result will be reactive energy. If you divide reactive energy by
 * reactive power, you get time, with the reactive tag removed.
 */
template<typename T1, typename T2> using mul_tag_t = typename detail::mul_tag<T1,T2>::type;
template<typename T1, typename T2> using div_tag_t = typename detail::div_tag<T1,T2>::type;
/** @} */

/* quantities ****************************************************************/

/**
 * @brief Quantity type
 *
 * This is a container for a value of ValueType, of the physical unit Unit,
 * with the scale Scale, and the tag Tag. It is meant to be used mostly like
 * ValueType would be used. For that, it overrides the necessary operators of
 * the built-in numeric types.
 *
 * @tparam       Unit   the quantity's unit type
 * @tparam      Scale   the quantity's scale
 * @tparam  ValueType   the quantity's value type
 * @tparam        Tag   the quantity's tag
 */
template< typename Unit
        , typename Scale     = no_scaling
        , typename ValueType = double
        , typename Tag       = no_tag >
class quantity;

namespace detail {

template<typename NewScale, typename OldScale>
struct value_rescaler {
	using conversion_scale = std::ratio_divide<OldScale, NewScale>;

	template<typename ValueType>
	constexpr static ValueType rescale_value(ValueType v) {
		const auto num = conversion_scale::num;
		const auto den = conversion_scale::den;
		const auto result = static_cast<ValueType>((v*num)/den);
		return result;
	}
};
template<typename Scale>
struct value_rescaler<Scale,Scale> {
	template<typename ValueType>
	static constexpr ValueType rescale_value(ValueType v)                 {return v;}
};

template<typename NewScale, typename OldScale, typename ValueType>
constexpr ValueType rescale_value(ValueType v)                            {return value_rescaler<NewScale,OldScale>::rescale_value(v);}

}

/**
 * @brief Quantity wrapper to allow specific implicit conversions.
 *
 * This type can be passed to a quantity and initiates the appropriate
 * conversions between different quantities.
 *
 * @tparam             U  quantity's unit
 * @tparam             S  quantity's scale
 * @tparam             V  quantity's value_type
 * @tparam             T  quantity's tag
 * @tparam ImplicitValue  allow conversions that change ValueType
 * @tparam ImplicitScale  allow conversions that change the unit's scale
 * @tparam   ImplicitTag  allow conversions that change the unit's tag
 *
 */
template< typename U, typename S, typename V, typename T
        , bool ImplicitValue, bool ImplicitScale, bool ImplicitTag >
class implicit_quantity_caster : protected quantity<U,S,V,T> {
public:
	explicit constexpr implicit_quantity_caster(const quantity<U,S,V,T>& q)
	                                                                    : quantity<U,S,V,T>{q} {}

	template<typename NewUnit, typename NewScale, typename NewValueType, typename NewTag>
	constexpr NewValueType cast_to() const {
		static_assert(                 are_units_compatible_v<U, NewUnit>     , "fundamentally incompatible units"      );
		static_assert(ImplicitScale || detail::is_same_v     <S, NewScale    >, "different unit scales (use scale_cast)");
		static_assert(ImplicitValue || detail::is_same_v     <V, NewValueType>, "different value types (use value_cast)");
		static_assert(ImplicitTag   || are_tags_compatible_v <T, NewTag>      , "different unit tags (use tag_cast)"    );

		return detail::rescale_value<NewScale, S>( static_cast<NewValueType>(this->get()) );
	}
};


/**
 * @brief Quantity instantiated with tag name
 *
 * This specialization allows the creation of quantities passing only a tag
 * name, rather than a real tag (which consists of the name and a ratio).
 *
 * @tparam       Unit   the quantity's unit type
 * @tparam      Scale   the quantity's scale
 * @tparam  ValueType   the quantity's value type
 * @tparam      TagID   the quantity's tag ID
 */
template< typename Unit
        , typename Scale
        , typename ValueType
        , typename TagID >
class quantity : public quantity<Unit, Scale, ValueType, create_tag_t<TagID>> {
public:
	using quantity<Unit, Scale, ValueType, create_tag_t<TagID>>::quantity;
};

/**
 * @brief Quantity instantiated with tag
 *
 * This specialization allows the creation of quantities with a real tag,
 * rather than just a tag name.
 *
 * @tparam       Unit   the quantity's unit type
 * @tparam      Scale   the quantity's scale
 * @tparam  ValueType   the quantity's value type
 * @tparam        Tag   the quantity's tag
 */
template< typename Unit
        , typename Scale
        , typename ValueType
        , typename TagID
        , std::intmax_t TagNum
        , std::intmax_t TagDen >
class quantity<Unit, Scale, ValueType, tag<TagID,TagNum,TagDen>> {
public:
	using unit_type  = Unit;                      /**< the quantity's unit type  */
	using scale_type = Scale;                     /**< the quantity's scale      */
	using value_type = ValueType;                 /**< the quantity's value type */
	using tag_type   = tag<TagID,TagNum,TagDen>;  /**< the quantity's tag        */

	/**
	 * @{
	 *
	 * shortcuts for the unit's nested types
	 */
	using             time_exponent = typename unit_type::            time_exponent;
	using             mass_exponent = typename unit_type::            mass_exponent;
	using           length_exponent = typename unit_type::          length_exponent;
	using          current_exponent = typename unit_type::         current_exponent;
	using       luminosity_exponent = typename unit_type::      luminosity_exponent;
	using      temperature_exponent = typename unit_type::     temperature_exponent;
	using substance_amount_exponent = typename unit_type::substance_amount_exponent;
	/** @} */

	/** result in R if T is an integer type, SFINAE otherwise */
	template<typename T, typename R>
	using if_integer = std::enable_if_t<std::numeric_limits<T>::is_integer, R>;

	/** create a quantity type with a different tag */
	template<typename NewTag>       using    retag   = quantity< unit_type, scale_type, value_type  , create_tag_t<NewTag>   >;
	                                using    untag   = quantity< unit_type, scale_type, value_type  , no_tag                 >;

	/** create a quantity with a different value type */
	template<typename NewValueType> using  revalue   = quantity< unit_type, scale_type, NewValueType, create_tag_t<tag_type> >;

	/** @{ create a quantity with a different value type */
	template<typename NewScale>     using rescale_to = quantity< unit_type, NewScale  , value_type  , create_tag_t<tag_type> >;
	template<typename NewScale>     using rescale_by = rescale_to<std::ratio_multiply<NewScale,scale_type>>;
	/** @} */

	constexpr quantity()                                                : value{} {}
	constexpr quantity(const quantity& rhs)                             = default;

	/**
	 * @brief Constructor
	 *
	 * Creates a quantity from a dimensionless value.
	 *
	 * @param v  Value to create quantity from
	 */
	constexpr explicit quantity(const value_type v)                     : value{v} {}

	/**
	 * @brief Conversion constructor
	 *
	 * The only conversion this constructor only allows is re-scaling of
	 * values.
	 *
	 * @note Rather than just disallowing conversion from other quantities
	 *       which would lead to hard-to-decipher template errors, this
	 *       converting constructor instead explicitly static_asserts the
	 *       properties that must be preserved. This produces error messages
	 *       which tell the user why the conversion is not allowed, and what
	 *       casts (if any) can be used to allow it.
	 *
	 * @param rhs  quantity to convert from
	 */
	template<typename OtherUnit, typename OtherScale, typename OtherValueType, typename OtherTag>
	constexpr quantity(const quantity<OtherUnit, OtherScale, OtherValueType, OtherTag>& rhs)
	: value{detail::rescale_value<scale_type,OtherScale>(rhs.get())}
	{
		static_assert(are_units_compatible_v<unit_type, OtherUnit      >, "fundamentally incompatible units"      );
		static_assert(detail::is_same_v     <value_type, OtherValueType>, "different value types (use value_cast)");
		static_assert(are_tags_compatible_v <tag_type  , OtherTag      >, "different unit tags (use tag_cast)"    );
	}

	/**
	 * @brief Casting constructor
	 *
	 * This allows the construction of a quantity from the result of a call
	 * to unlibs::{value, scale, unit, quantity}_cast(). It performs implicit
	 * conversions of any combination of scale, tag, and value type.
	 *
	 * @param rhs  Quantity to convert from
	 *
	 * @sa unlibs::value_cast
	 */
	template<typename OtherUnit, typename OtherScale, typename OtherValueType, typename OtherTag, bool IV, bool IS, bool IT>
	constexpr quantity(const implicit_quantity_caster<OtherUnit, OtherScale, OtherValueType, OtherTag, IV, IS, IT>& rhs)
	                                                                    : value{ rhs.template cast_to<unit_type,scale_type,value_type,tag_type>() } {}

	/**
	 * @brief Casting assignment operator
	 *
	 * This allows the assignment to a quantity from the result of a call
	 * to unlibs::{value, scale, unit, quantity}_cast(). It performs implicit
	 * conversions of any combination of scale, tag, and value type.
	 *
	 * @param rhs  Quantity to convert from
	 *
	 * @sa unlibs::value_cast
	 */
	template<typename OtherUnit, typename OtherScale, typename OtherValueType, typename OtherTag, bool IV, bool IS, bool IT>
	constexpr quantity& operator=(const implicit_quantity_caster<OtherUnit, OtherScale, OtherValueType, OtherTag, IV, IS, IT>& rhs)
	                                                                      {value = rhs.template cast_to<unit_type,scale_type,value_type,tag_type>(); return *this;}

	constexpr quantity& operator=(const quantity& rhs)                  = default;

	/**
	 * @{
	 *
	 * @brief get value
	 *
	 * This retrieves the quantity's underlying value
	 */
	constexpr value_type get() const                                      {return value;}
	friend constexpr value_type get(const quantity& q)                    {return q.get();}
	/** @} */

	/**
	 * @{
	 *
	 * @brief get scaled value
	 *
	 * This retrieves the quantity's underlying value at the specified scale
	 */
	template<typename NewScale>
	constexpr value_type get_scaled(NewScale = NewScale{}) const          {return detail::rescale_value<NewScale,scale_type>(value);}
	template<typename NewScale, typename U, typename S, typename V, typename T, typename E>
	constexpr value_type get_scaled(const quantity<U,S,V,T>& q, NewScale = NewScale{})
	                                                                      {return q.template get_scaled<NewScale>();}
	/** @} */

	/**
	 * @{
	 * @brief Comparison operators
	 */
	template<typename OtherScale> constexpr bool operator==(const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return value == rhs.template get_scaled<scale_type>();}
	template<typename OtherScale> constexpr bool operator< (const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return value <  rhs.template get_scaled<scale_type>();}
	template<typename OtherScale> constexpr bool operator> (const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return value >  rhs.template get_scaled<scale_type>();}
	template<typename OtherScale> constexpr bool operator>=(const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return not (*this <  rhs);}
	template<typename OtherScale> constexpr bool operator<=(const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return not (*this >  rhs);}
	template<typename OtherScale> constexpr bool operator!=(const quantity<unit_type,OtherScale,value_type,tag_type>& rhs) const {return not (*this == rhs);}
	/** @} */

	/**
	 * @{
	 * @brief Compound mathematical operators
	 *
	 * @param rhs  other quantity to combine with
	 *
	 * @return self
	 */
	template<typename U, typename S, typename V, typename T>
	constexpr quantity& operator+=(const quantity<U,S,V,T>& rhs)          {
		                                                                      static_assert(are_units_compatible_v<unit_type , U>, "fundamentally incompatible units"      );
		                                                                      static_assert(detail::is_same_v     <value_type, V>, "different value types (use value_cast)");
		                                                                      static_assert(are_tags_compatible_v <tag_type  , T>, "incompatible tags (use tag_cast)"      );
		                                                                      value += rhs.template get_scaled<scale_type>();
		                                                                      return *this;
	                                                                      }
	template<typename U, typename S, typename V, typename T>
	constexpr quantity& operator-=(const quantity<U,S,V,T>& rhs)          {
		                                                                      static_assert(are_units_compatible_v<unit_type , U>, "fundamentally incompatible units"      );
		                                                                      static_assert(detail::is_same_v     <value_type, V>, "different value types (use value_cast)");
		                                                                      static_assert(are_tags_compatible_v <tag_type  , T>, "incompatible tags (use tag_cast)"      );
		                                                                      value -= rhs.template get_scaled<scale_type>();
		                                                                      return *this;
	                                                                      }

	template<typename V>
	constexpr quantity& operator*=(const V& rhs)                          {
		                                                                      static_assert(detail::is_same_v<value_type, V>, "different value types");
		                                                                      value *= rhs;
		                                                                      return *this;
	                                                                      }
	template<typename V>
	constexpr quantity& operator/=(const V& rhs)                          {
		                                                                      static_assert(detail::is_same_v<value_type, V>, "different value types");
		                                                                      value /= rhs;
		                                                                      return *this;
	                                                                      }
	template<typename V>
	constexpr if_integer<V,quantity>& operator%=(const V& rhs)            {
		                                                                      static_assert(detail::is_same_v<value_type, V>, "different value types");
		                                                                      value %= rhs;
		                                                                      return *this;
	                                                                      }
	/** @} */

	/**
	 * @brief Unary minus.
	 *
	 * @note Unary minus on unsigned ValueTypes is disallowed by static_assert.
	 *
	 * @return negated copy of self
	 */
	constexpr quantity operator-() const                                  {
		                                                                      static_assert(std::is_signed<value_type>::value, "unary minus on unsigned");
		                                                                      return quantity{-value};
	                                                                      }

	/**
	 * @brief Unary plus.
	 *
	 * @return self
	 */
	constexpr quantity operator+() const                                  {return *this;}

	/**
	 * @{
	 *
	 * @brief Check whether two floating point quantities are almost equal
	 *
	 * Checks whether two floating point quantities are almost equal. For
	 * integer types, this will test whether they are equal.
	 *
	 * @param epsilon  Tolerance for comparison.
	 *
	 * @return true, if near zero
	 */
	template<typename U, typename S, typename V, typename T, typename E = value_type>
	constexpr bool is_near(const quantity<U,S,V,T>& rhs, const E epsilon = get_epsilon<V>()) const
	                                                                      {
		                                                                      static_assert(are_units_compatible_v<unit_type , U>, "fundamentally incompatible units"      );
		                                                                      static_assert(detail::is_same_v     <value_type, V>, "different value types (use value_cast)");
		                                                                      static_assert(are_tags_compatible_v <tag_type  , T>, "incompatible tags (use tag_cast)"      );
		                                                                      return std::abs(value - rhs.template get_scaled<scale_type>()) <= epsilon;
	                                                                      }
	template<typename U, typename S, typename V, typename T, typename E = value_type>
	friend constexpr bool is_near(const quantity& lhs, const quantity<U,S,V,T>& rhs, const E epsilon = get_epsilon<value_type>())
	                                                                      {return lhs.is_near(rhs,epsilon);}
	/** @} */

	/**
	 * @{
	 *
	 * @brief Check whether a floating point quantity is near zero
	 *
	 * Checks whether a floating point value is near zero. For integer types,
	 * this will test whether the value is zero.
	 *
	 * @param epsilon  Tolerance for comparison.
	 *
	 * @return true, if near zero
	 */
	 template<typename E = value_type>
	constexpr bool is_near_zero(const E epsilon = get_epsilon<value_type>()) const
	                                                                      {return std::abs(value) <= epsilon;}
	template<typename E = value_type>
	friend constexpr bool is_near_zero(const quantity& q, const E epsilon = get_epsilon<value_type>())
	                                                                      {return q.is_near_zero(epsilon);}
	/** @} */

private:
	template<typename V>
	static constexpr V get_epsilon()                                      {return std::numeric_limits<V>::epsilon() > V{} ? 100*std::numeric_limits<V>::epsilon() : V{};}

	value_type                                        value;
};


/* quantity multiplication/division type manipulations ***********************/

/**
 * @{
 *
 * Calculate the types needed when multiplying/dividing quantities.
 */
template<typename Q1, typename Q2> using mul_quantity_t = quantity< mul_unit_t<typename Q1::unit_type, typename Q2::unit_type>
                                                                  , std::ratio_multiply<typename Q1::scale_type, typename Q2::scale_type>
                                                                  , decltype(typename Q1::value_type{}*typename Q2::value_type{})
                                                                  , mul_tag_t<typename Q1::tag_type, typename Q2::tag_type> >;
template<typename Q1, typename Q2> using div_quantity_t = quantity< div_unit_t<typename Q1::unit_type, typename Q2::unit_type>
                                                                  , std::ratio_divide<typename Q1::scale_type, typename Q2::scale_type>
                                                                  , decltype(typename Q1::value_type{}/typename Q2::value_type{})
                                                                  , div_tag_t<typename Q1::tag_type, typename Q2::tag_type> >;
template<typename Q> using sqrt_quantity_t = quantity< sqrt_unit_t<typename Q::unit_type>
                                                     , typename Q::scale_type
                                                     , typename Q::value_type
                                                     , create_tag_t< typename Q::tag_type::id
                                                                   , std::ratio_divide<typename Q::tag_type::exponent, std::ratio<2,1>>::num
                                                                   , std::ratio_divide<typename Q::tag_type::exponent, std::ratio<2,1>>::den
                                                                   >
                                                     >;
/** @} */


/* quantity scaling **********************************************************/

namespace detail {

template<typename NewScale, typename ScaleOrQuantity> struct scale_by;
template<typename NewScale, typename ScaleOrQuantity> struct scale_to;

template<typename NewScale, typename U, typename S, typename V, typename T>
struct scale_by<NewScale, quantity<U,S,V,T>>                    {using result = typename quantity<U,S,V,T>::template rescale_by<NewScale>;};
template<typename NewScale, typename U, typename S, typename V, typename T>
struct scale_to<NewScale, quantity<U,S,V,T>>                    {using result = typename quantity<U,S,V,T>::template rescale_to<NewScale>;};

template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_by<NewScale, std::ratio<Num,Den>>                  {using result = std::ratio_multiply<NewScale,std::ratio<Num,Den>>;};
template<typename NewScale, std::intmax_t Num, std::intmax_t Den >
struct scale_to<NewScale, std::ratio<Num,Den>>                  {using result = NewScale;};

}

/**
 * @{
 *
 * Meta functions to get the type of a specifically scaled quantity.
 *
 * @note micro<kilo<meter>> will result in milli<meter>,
 *       to_micro<to_kilo<meter>> will result in micro<meter>.
 */
template<typename ScaleOrQuantity> using    second_scale = ScaleOrQuantity;
template<typename ScaleOrQuantity> using    minute_scale = typename detail::scale_by<minute_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      hour_scale = typename detail::scale_by<  hour_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using       day_scale = typename detail::scale_by<   day_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      week_scale = typename detail::scale_by<  week_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      atto       = typename detail::scale_by<  atto_scaling,ScaleOrQuantity>::result;

template<typename ScaleOrQuantity> using     femto       = typename detail::scale_by< femto_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      pico       = typename detail::scale_by<  pico_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      nano       = typename detail::scale_by<  nano_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using     micro       = typename detail::scale_by< micro_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using     milli       = typename detail::scale_by< milli_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using     centi       = typename detail::scale_by< centi_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      deci       = typename detail::scale_by<  deci_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using  no_scale       = ScaleOrQuantity;
template<typename ScaleOrQuantity> using      deca       = typename detail::scale_by<  deca_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using     hecto       = typename detail::scale_by< hecto_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      kilo       = typename detail::scale_by<  kilo_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      mega       = typename detail::scale_by<  mega_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      giga       = typename detail::scale_by<  giga_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      tera       = typename detail::scale_by<  tera_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using      peta       = typename detail::scale_by<  peta_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using       exa       = typename detail::scale_by<   exa_scaling,ScaleOrQuantity>::result;

template<typename ScaleOrQuantity> using to_second_scale = no_scale<ScaleOrQuantity>;
template<typename ScaleOrQuantity> using to_minute_scale = typename detail::scale_to<minute_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_hour_scale   = typename detail::scale_to<  hour_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_day_scale    = typename detail::scale_to<   day_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_week_scale   = typename detail::scale_to<  week_scaling,ScaleOrQuantity>::result;

template<typename ScaleOrQuantity> using to_atto         = typename detail::scale_to<  atto_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_femto        = typename detail::scale_to< femto_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_pico         = typename detail::scale_to<  pico_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_nano         = typename detail::scale_to<  nano_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_micro        = typename detail::scale_to< micro_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_milli        = typename detail::scale_to< milli_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_centi        = typename detail::scale_to< centi_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_deci         = typename detail::scale_to<  deci_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_no_scale     = no_scale<ScaleOrQuantity>;
template<typename ScaleOrQuantity> using to_deca         = typename detail::scale_to<  deca_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_hecto        = typename detail::scale_to< hecto_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_kilo         = typename detail::scale_to<  kilo_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_mega         = typename detail::scale_to<  mega_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_giga         = typename detail::scale_to<  giga_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_tera         = typename detail::scale_to<  tera_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_peta         = typename detail::scale_to<  peta_scaling,ScaleOrQuantity>::result;
template<typename ScaleOrQuantity> using to_exa          = typename detail::scale_to<   exa_scaling,ScaleOrQuantity>::result;
/** @} */


/* quantity operators ********************************************************/

/**
 * @brief Stream output operator
 *
 * Streams the stored value into os.
 *
 * @param os  the stream
 * @param  q  the quantity
 * @return os
 */
template<typename U, typename S, typename V, typename T>
std::ostream& operator<<(std::ostream& os, const quantity<U, S, V, T>& q) {return os << q.get();}

/**
 * @brief Stream input operator
 *
 * Reads from the stream into q.
 *
 * @param is  the stream
 * @param  q  the quantity
 *
 * @return is
 */
template<typename U, typename S, typename V, typename T>
std::istream& operator>>(std::istream& is, quantity<U, S, V, T>& q)       {
	                                                                          V v;
	                                                                          if (is >> v)
		                                                                          q = quantity<U, S, V, T>{v};
	                                                                          return is;
                                                                          }

/**
 * @{
 * @brief Non-member binary arithmetic operators
 *
 * Combine two quantities and produce a new one,
 * whose type is derived from the inputs.
 *
 * @example meter{24} / second{6} == meter_per_second{4}
 *
 * @param lhs  quantity or dimensionless value
 * @param rhs  quantity or dimensionless value
 * @return combined result
 */
template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator+(quantity<U1,S1,V1,T1> lhs, const quantity<U2,S2,V2,T2>& rhs)
                                                                          {return lhs += rhs;}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator-(quantity<U1,S1,V1,T1> lhs, const quantity<U2,S2,V2,T2>& rhs)
                                                                          {return lhs -= rhs;}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator*( const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return mul_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() * rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator*( const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}*V2{}),T1>{lhs.get() * rhs};
}
template<typename V1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator*( const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	return quantity<U2,S2,decltype(V1{}*V2{}),T2>{lhs * rhs.get()};
}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator/( const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return div_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() / rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator/( const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}/V2{}),T1>{lhs.get() / rhs};
}
template<typename V1, typename U2, typename S2, typename T2, typename V2>
constexpr auto operator/( const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	return quantity<reciprocal_unit_t<U2>,S2,decltype(V1{}/V2{}),T2>{lhs / rhs.get()};
}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator%( const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return div_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() % rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator%( const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}%V2{}),T1>{lhs.get() % rhs};
}
template<typename V1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator%( const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	return quantity<reciprocal_unit_t<U2>,S2,decltype(V1{}%V2{}),T2>{lhs % rhs.get()};
}
/** @} */


/* quantity casts ************************************************************/

/**
 * @{
 * @brief Cast between different ValueTypes
 *
 * Performs casts between quantities of compatible units with different
 * value types.
 *
 * @param             q  quantity to cast
 * @tparam NewValueType  (optional) the value type to cast to
 *
 * @return q with same unit, but value static_cast<>'ed to NewValueType
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the scale conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * seconds<std::int16_t> value =                          1_s ; // error, milli<seconds<unsigned long long>>
 * seconds<std::int16_t> value = value_cast<std::int16_t>(1_s); // OK
 * seconds<std::int16_t> value = value_cast              (1_s); // OK
 *
 */
template<typename V, typename U, typename S, typename T>
constexpr auto value_cast(const quantity<U,S,V,T>& q)                     {return implicit_quantity_caster<U,S,V,T,true,false,false>{q};}
template<typename NewValueType, typename U, typename S, typename T, typename ValueType>
constexpr auto value_cast(const quantity<U,S,ValueType,T>& q)             {return quantity<U,S,NewValueType,T>{value_cast(q)};}
/**@} */

/**
 * @{
 * @brief Cast between differently scaled quantities
 *
 * Performs casts between related, compatible quantities with different scales,
 * e.g. from millimeters to meters.
 *
 * @param         q  quantity to cast
 * @tparam NewScale  (optional) scale to cast to, e.g. milli_scaling
 * @return q with rescaled value and changed scale
 *
 * @note Scale conversions are also allowed implicitly.
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the scale conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * milli<seconds<unsigned long long>> value =                           1_s ; // error, seconds<unsigned long long>
 * milli<seconds<unsigned long long>> value = scale_cast<milli_scaling>(1_s); // OK
 * milli<seconds<unsigned long long>> value = scale_cast               (1_s); // OK
 */
template<typename S, typename U, typename ValueType, typename T>
constexpr auto scale_cast(const quantity<U,S,ValueType,T>& q)             {return implicit_quantity_caster<U,S,ValueType,T,false,true,false>{q};}
template<typename NewScale, typename U, typename S, typename ValueType, typename T>
constexpr auto scale_cast(const quantity<U,S,ValueType,T>& q)             {return typename quantity<U,S,ValueType,T>::template rescale_by<NewScale>{scale_cast(q)};}
/** @} */


/**
 * @{
 * @brief Cast between quantities with different tags
 *
 * Performs casts between quantities of compatible units with different
 * tags.
 *
 * @param       q  quantity to cast
 * @tparam NewTag  (optional)  unit to cast to
 * @return q with changed tag
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the scale conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * kilo<voltampere<double>> value =                                1_kVAr ; // error, kilo<var<double>>
 * kilo<voltampere<double>> value = tag_cast<voltampere::tag_type>(1_kVAr); // OK
 * kilo<voltampere<double>> value = tag_cast                      (1_kVAr); // OK
 */
template<typename U, typename S, typename ValueType, typename T>
constexpr auto tag_cast(const quantity<U,S,ValueType,T>& q)               {return implicit_quantity_caster<U,S,ValueType,T,false,false,true>{q};}
template<typename NewTag, typename U, typename S, typename ValueType, typename T>
constexpr auto tag_cast(const quantity<U,S,ValueType,T>& q)               {return quantity<U,S,ValueType,create_tag_t<NewTag>>{tag_cast(q)};}
/** @} */


/**
 * @{
 * @brief Cast between between quantities with different scales, value types and/or tags
 *
 * Performs casts between quantities of compatible units with different
 * scales, value types and/or tags.
 *
 * @param            q  quantity to cast
 * @tparam NewQuantity  quantity to cast to
 * @return q converted to NewQuantity
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the scale conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * kilo<voltampere<float>> value =                                 1_MVAr ; // error, mega<var<double>>
 * kilo<voltampere<float>> value = quantity_cast<kilo<voltampere>>(1_MVAr); // ok
 * kilo<voltampere<float>> value = quantity_cast                  (1_MVAr); // ok
 */
template<typename U, typename S, typename ValueType, typename T>
constexpr auto quantity_cast(const quantity<U,S,ValueType,T>& q)          {return implicit_quantity_caster<U,S,ValueType,T,true,true,true>{q};}
template<typename NewQuantity, typename U, typename S, typename ValueType, typename T>
constexpr NewQuantity quantity_cast(const quantity<U,S,ValueType,T>& q)   {return NewQuantity{quantity_cast(q)};}
/** @} */

}

#endif /* UNLIB_QUANTITY_HPP */
