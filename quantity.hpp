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

#include <cstdint>
#include <ostream>
#include <istream>
#include <limits>
#include <type_traits>

#include <unlib/unit.hpp>
#include <unlib/scaling.hpp>
#include <unlib/tag.hpp>


namespace unlib {

namespace detail {
/* workaround for C++14 lacking this */
template<typename T1, typename T2>
constexpr bool is_same_v = std::is_same<T1,T2>::value;
}

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
 * @brief Quantity instantiated with tag
 *
 * This specialization assures quantities are created with tags.
 *
 * @tparam       Unit   the quantity's unit type
 * @tparam      Scale   the quantity's scale
 * @tparam  ValueType   the quantity's value type
 * @tparam        Tag   the quantity's tag
 * @tparam      TagID   the quantity's tag ID
 * @tparam     TagNum   the quantity's tag exponents numerator
 * @tparam     TagDen   the quantity's tag exponents denominator
 */
template< std::intmax_t            TimeNum, std::intmax_t            TimeDen
        , std::intmax_t            MassNum, std::intmax_t            MassDen
        , std::intmax_t          LengthNum, std::intmax_t          LengthDen
        , std::intmax_t         CurrentNum, std::intmax_t         CurrentDen
        , std::intmax_t      LuminosityNum, std::intmax_t      LuminosityDen
        , std::intmax_t     TemperatureNum, std::intmax_t     TemperatureDen
        , std::intmax_t SubstanceAmountNum, std::intmax_t SubstanceAmountDen
        , std::intmax_t           ScaleNum, std::intmax_t           ScaleDen
        , std::intmax_t             TagNum, std::intmax_t             TagDen
        , typename TagID
        , typename ValueType >
class quantity< unit< std::ratio<           TimeNum,           TimeDen>
                    , std::ratio<           MassNum,           MassDen>
                    , std::ratio<         LengthNum,         LengthDen>
                    , std::ratio<        CurrentNum,        CurrentDen>
                    , std::ratio<     LuminosityNum,     LuminosityDen>
                    , std::ratio<    TemperatureNum,    TemperatureDen>
                    , std::ratio<SubstanceAmountNum,SubstanceAmountDen> >
              , std::ratio<ScaleNum,ScaleDen>
              , ValueType
              , tag<TagID,std::ratio<TagNum,TagDen>> > {
public:
	using unit_type  = unit< std::ratio<           TimeNum,           TimeDen>    /**< the quantity's unit type  */
	                       , std::ratio<           MassNum,           MassDen>
	                       , std::ratio<         LengthNum,         LengthDen>
	                       , std::ratio<        CurrentNum,        CurrentDen>
	                       , std::ratio<     LuminosityNum,     LuminosityDen>
	                       , std::ratio<    TemperatureNum,    TemperatureDen>
	                       , std::ratio<SubstanceAmountNum,SubstanceAmountDen> >;
	using scale_type = scale_t<ScaleNum,ScaleDen>;                                /**< the quantity's scale      */
	using value_type = ValueType;                                                 /**< the quantity's value type */
	using tag_type   = tag_t<TagID,TagNum,TagDen>;                                /**< the quantity's tag        */

	/**
	 * @{
	 *
	 * shortcuts for the unit's nested types
	 */
	using             time_exponent =             time_exponent_t<unit_type>;
	using             mass_exponent =             mass_exponent_t<unit_type>;
	using           length_exponent =           length_exponent_t<unit_type>;
	using          current_exponent =          current_exponent_t<unit_type>;
	using       luminosity_exponent =       luminosity_exponent_t<unit_type>;
	using      temperature_exponent =      temperature_exponent_t<unit_type>;
	using substance_amount_exponent = substance_amount_exponent_t<unit_type>;
	/** @} */

	/** create a quantity type with a different tag */
	template<typename NewTag>       using    retag   = quantity< unit_type, scale_type, value_type  , NewTag   >;
	                                using untag      = quantity< unit_type, scale_type, value_type  , no_tag                 >;

	/** create a quantity with a different value type */
	template<typename NewValueType> using  revalue   = quantity< unit_type, scale_type, NewValueType, tag_type >;

	/** @{ create a quantity with a different value type */
	template<typename NewScale>     using rescale_to = quantity< unit_type, NewScale  , value_type  , tag_type >;
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
	template<typename U, typename S, typename V, typename T>
	constexpr quantity(const quantity<U, S, V, T>& rhs)
	: value{detail::rescale_value<scale_type,S>(rhs.get())}
	{
		static_assert(are_units_compatible_v<unit_type , U>, "fundamentally incompatible units"      );
		static_assert(detail::is_same_v     <value_type, V>, "different value types (use value_cast)");
		static_assert(detail::is_same_v     <tag_type  , T>, "different unit tags (use tag_cast)"    );
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
	template<typename U, typename S, typename V, typename T, bool IV, bool IS, bool IT>
	constexpr quantity(const implicit_quantity_caster<U, S, V, T, IV, IS, IT>& rhs)
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
	template<typename U, typename S, typename V, typename T, bool IV, bool IS, bool IT>
	constexpr quantity& operator=(const implicit_quantity_caster<U, S, V, T, IV, IS, IT>& rhs)
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
	template<typename S> constexpr bool operator==(const quantity<unit_type,S,value_type,tag_type>& rhs) const {return value == rhs.template get_scaled<scale_type>();}
	template<typename S> constexpr bool operator< (const quantity<unit_type,S,value_type,tag_type>& rhs) const {return value <  rhs.template get_scaled<scale_type>();}
	template<typename S> constexpr bool operator> (const quantity<unit_type,S,value_type,tag_type>& rhs) const {return value >  rhs.template get_scaled<scale_type>();}
	template<typename S> constexpr bool operator>=(const quantity<unit_type,S,value_type,tag_type>& rhs) const {return not (*this <  rhs);}
	template<typename S> constexpr bool operator<=(const quantity<unit_type,S,value_type,tag_type>& rhs) const {return not (*this >  rhs);}
	template<typename S> constexpr bool operator!=(const quantity<unit_type,S,value_type,tag_type>& rhs) const {return not (*this == rhs);}
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
		                                                                      static_assert(detail::is_same_v     <tag_type  , T>, "incompatible tags (use tag_cast)"      );
		                                                                      value += rhs.template get_scaled<scale_type>();
		                                                                      return *this;
	                                                                      }
	template<typename U, typename S, typename V, typename T>
	constexpr quantity& operator-=(const quantity<U,S,V,T>& rhs)          {
		                                                                      static_assert(are_units_compatible_v<unit_type , U>, "fundamentally incompatible units"      );
		                                                                      static_assert(detail::is_same_v     <value_type, V>, "different value types (use value_cast)");
		                                                                      static_assert(detail::is_same_v     <tag_type  , T>, "incompatible tags (use tag_cast)"      );
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
	constexpr quantity& operator%=(const V& rhs)                          {
		                                                                      static_assert(std::numeric_limits<value_type>::is_integer, "modulo on non-integer type"  );
		                                                                      static_assert(std::numeric_limits<V         >::is_integer, "modulo with non-integer type");
		                                                                      static_assert(detail::is_same_v<value_type,V>            , "different value types"       );
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
		                                                                      static_assert(detail::is_same_v     <tag_type  , T>, "incompatible tags (use tag_cast)"      );
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

/**
 * @{
 *
 * @brief Result type for multiplying/dividing value types
 */
template<typename V1, typename V2> using mul_value_t = decltype(V1{}*V2{});
template<typename V1, typename V2> using div_value_t = decltype(V1{}/V2{});
/** @} */

/* quantity type manipulations ***********************************************/

/**
 * @{
 *
 * Calculate the types needed when multiplying/dividing quantities.
 */
template<typename Q1, typename Q2> using  mul_quantity_t = quantity< mul_unit_t <typename Q1::unit_type , typename Q2::unit_type >
                                                                   , mul_scale_t<typename Q1::scale_type, typename Q2::scale_type>
                                                                   , mul_value_t<typename Q1::value_type, typename Q2::value_type>
                                                                   , mul_tag_t  <typename Q1::tag_type  , typename Q2::tag_type  > >;
template<typename Q1, typename Q2> using  div_quantity_t = quantity< div_unit_t <typename Q1::unit_type , typename Q2::unit_type >
                                                                   , div_scale_t<typename Q1::scale_type, typename Q2::scale_type>
                                                                   , div_value_t<typename Q1::value_type, typename Q2::value_type>
                                                                   , div_tag_t  <typename Q1::tag_type  , typename Q2::tag_type  > >;
template<typename Q              > using sqrt_quantity_t = quantity< sqrt_unit_t<typename Q::unit_type>
                                                                   , sqrt_scale_t<typename Q::scale_type>
                                                                   , typename Q::value_type
                                                                   , sqrt_tag_t<typename Q::tag_type> >;
template<typename Q , int Power  > using  pow_quantity_t = quantity< pow_unit_t<typename Q::unit_type, Power>
                                                                   , pow_scale_t<typename Q::scale_type,Power>
                                                                   , typename Q::value_type
                                                                   , pow_tag_t<typename Q::tag_type, Power> >;
/** @} */


/* quantity scaling **********************************************************/

namespace detail {

/* This allows quantities to be scaled using milli<q> and to_milli<q> */
template<typename NewScale, typename U, typename S, typename V, typename T>
struct scale_by<NewScale, quantity<U,S,V,T>>                    {using result = typename quantity<U,S,V,T>::template rescale_by<NewScale>;};
template<typename NewScale, typename U, typename S, typename V, typename T>
struct scale_to<NewScale, quantity<U,S,V,T>>                    {using result = typename quantity<U,S,V,T>::template rescale_to<NewScale>;};

}


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
constexpr auto operator*(const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return mul_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() * rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator*(const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}*V2{}),T1>{lhs.get() * rhs};
}
template<typename V1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator*( const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	return quantity<U2,S2,decltype(V1{}*V2{}),T2>{lhs * rhs.get()};
}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator/(const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return div_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() / rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator/(const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}/V2{}),T1>{lhs.get() / rhs};
}
template<typename V1, typename U2, typename S2, typename T2, typename V2>
constexpr auto operator/( const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	return quantity<reciprocal_unit_t<U2>,S2,decltype(V1{}/V2{}),T2>{lhs / rhs.get()};
}

template<typename U1, typename S1, typename V1, typename T1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator%(const quantity<U1,S1,V1,T1>& lhs, const quantity<U2,S2,V2,T2>& rhs) {
	static_assert(detail::is_same_v    <V1, V2>, "different value types (use value_cast)");
	static_assert(are_tags_compatible_v<T1, T2>, "incompatible tags (use tag_cast)"      );
	return div_quantity_t<quantity<U1,S1,V1,T1>, quantity<U2,S2,V2,T2>>{lhs.get() % rhs.get()};
}
template<typename U1, typename S1, typename V1, typename T1, typename V2>
constexpr auto operator%(const quantity<U1,S1,V1,T1>& lhs, const V2& rhs) {
	return quantity<U1,S1,decltype(V1{}%V2{}),T1>{lhs.get() % rhs};
}
template<typename V1, typename U2, typename S2, typename V2, typename T2>
constexpr auto operator%(const V1& lhs, const quantity<U2,S2,V2,T2>& rhs) {
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
template<typename NewValueType, typename U, typename S, typename T, typename V>
constexpr auto value_cast(const quantity<U,S,V,T>& q)                     {return quantity<U,S,NewValueType,T>{value_cast(q)};}
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
template<typename S, typename U, typename V, typename T>
constexpr auto scale_cast(const quantity<U,S,V,T>& q)                     {return implicit_quantity_caster<U,S,V,T,false,true,false>{q};}
template<typename NewScale, typename U, typename S, typename V, typename T>
constexpr auto scale_cast(const quantity<U,S,V,T>& q)                     {return typename quantity<U,S,V,T>::template rescale_by<NewScale>{scale_cast(q)};}
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
template<typename U, typename S, typename V, typename T>
constexpr auto tag_cast(const quantity<U,S,V,T>& q)                       {return implicit_quantity_caster<U,S,V,T,false,false,true>{q};}
template<typename NewTag, typename U, typename S, typename V, typename T>
constexpr auto tag_cast(const quantity<U,S,V,T>& q)                       {return quantity<U,S,V,NewTag>{tag_cast(q)};}
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
template<typename U, typename S, typename V, typename T>
constexpr auto quantity_cast(const quantity<U,S,V,T>& q)                  {return implicit_quantity_caster<U,S,V,T,true,true,true>{q};}
template<typename NewQuantity, typename U, typename S, typename V, typename T>
constexpr NewQuantity quantity_cast(const quantity<U,S,V,T>& q)           {return NewQuantity{quantity_cast(q)};}
/** @} */

}

#endif /* UNLIB_QUANTITY_HPP */
