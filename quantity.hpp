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

namespace detail {

template<typename NewScale, typename OldScale>
struct value_rescaler {
	using conversion_scale = std::ratio_divide<OldScale , NewScale>;

	template<typename ValueType>
	constexpr static ValueType rescale_value(ValueType v) {
		const auto num = conversion_scale::num;
		const auto den = conversion_scale::den;
		const auto factor = static_cast<double>(num) / den;
		return static_cast<ValueType>( v * factor );
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
 * @brief Quantity type
 *
 * This is a container for a value of ValueType, of the physical unit Unit,
 * with the scale Scale, and the tag Tag. It is meant to be used mostly like
 * ValueType would be used. For that, it overrides the appropriate operators
 * of the built-in numeric types.
 */
template< typename Unit
        , typename Scale      = no_scaling
        , typename ValueType  = double
        , typename Tag        = void >
class quantity;

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
		static_assert( are_units_compatible<U, NewUnit>::value              , "fundamentally incompatible units"      );
		static_assert( ImplicitScale || std::is_same<S, NewScale    >::value, "different unit scales (use scale_cast)");
		static_assert( ImplicitValue || std::is_same<V, NewValueType>::value, "different value types (use value_cast)");
		static_assert( ImplicitTag   || std::is_same<T, NewTag      >::value, "different unit tags (use tag_cast)"    );

		return detail::rescale_value<NewScale, S>( static_cast<NewValueType>(this->get()) );
	}
};


template< typename Unit
        , typename Scale
        , typename ValueType
        , typename Tag >
class quantity {
public:
	using unit_type    = Unit     ; /**< the quantity's unit type  */
	using scale_type   = Scale    ; /**< the quantity's scale      */
	using value_type   = ValueType; /**< the quantity's value type */
	using tag_type     = Tag      ; /**< the quantity's unique tag */

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
	using if_integer = std::enable_if_t< std::numeric_limits<T>::is_integer, R>;

	/** create a quantity type */
	template<typename NewTag>       using    retag   = quantity< unit_type, scale_type, value_type  , NewTag  >;

	/** re-define a quantity value type */
	template<typename NewValueType> using  revalue   = quantity< unit_type, scale_type, NewValueType, tag_type>;

	/**
	 * @{
	 * re-scale a quantity type
	 */
	template<typename NewScale>     using rescale_to = quantity< unit_type, NewScale  , value_type  , tag_type>;
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
		static_assert( are_units_compatible<unit_type, OtherUnit>::value, "fundamentally incompatible units"      );
		static_assert( std::is_same<value_type  , OtherValueType>::value, "different value types (use value_cast)");
		static_assert( std::is_same<tag_type    , OtherTag      >::value, "different unit tags (use tag_cast)"    );
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
	 * @brief get value
	 *
	 * This retrieves the quantity's underlying value
	 */
	constexpr value_type get() const                                      {return value;}

	/**
	 * @brief get scaled value
	 *
	 * This retrieves the quantity's underlying value at the specified scale
	 */
	template<typename NewScale>
	constexpr value_type get_scaled(NewScale = NewScale{}) const          {return detail::rescale_value<NewScale,scale_type>(value);}

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
		                                                                      static_assert( are_units_compatible<unit_type , U>::value, "fundamentally incompatible units"      );
		                                                                      static_assert( std::is_same        <value_type, V>::value, "different value types (use value_cast)");
		                                                                      static_assert( std::is_same        <tag_type  , T>::value, "incompatible tags (use tag_cast)"      );
		                                                                      value += rhs.template get_scaled<scale_type>();
		                                                                      return *this;
	                                                                      }
	template<typename U, typename S, typename V, typename T>
	constexpr quantity& operator-=(const quantity<U,S,V,T>& rhs)          {
		                                                                      static_assert( are_units_compatible<unit_type , U>::value, "fundamentally incompatible units"      );
		                                                                      static_assert( std::is_same        <value_type, V>::value, "different value types (use value_cast)");
		                                                                      static_assert( std::is_same        <tag_type  , T>::value, "incompatible tags (use tag_cast)"      );
		                                                                      value -= rhs.template get_scaled<scale_type>();
		                                                                      return *this;
	                                                                      }

	template<typename V>
	constexpr quantity& operator*=(const V& rhs)                          {
		                                                                      static_assert( std::is_same<value_type, V>::value, "different value types");
		                                                                      value *= rhs;
		                                                                      return *this;
	                                                                      }
	template<typename V>
	constexpr quantity& operator/=(const V& rhs)                          {
		                                                                      static_assert( std::is_same<value_type, V>::value, "different value types");
		                                                                      value /= rhs;
		                                                                      return *this;
	                                                                      }
	template<typename V>
	constexpr if_integer<V,quantity>& operator%=(const V& rhs)            {
		                                                                      static_assert( std::is_same<value_type, V>::value, "different value types");
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
	 * @brief Check whether two floating point quantities are almost equal
	 *
	 * Checks whether two floating point quantities are almost equal. For
	 * integer types, this will test whether they are equal.
	 *
	 * @param epsilon  Tolerance for comparison.
	 *
	 * @return true, if near zero
	 */
	template<typename U, typename S, typename V, typename T>
	constexpr bool is_near(const quantity<U,S,V,T>& rhs, const ValueType epsilon = std::numeric_limits<value_type>::epsilon()) const
	                                                                      {
		                                                                      static_assert( are_units_compatible<unit_type , U>::value, "fundamentally incompatible units");
		                                                                      static_assert( std::is_same        <value_type, V>::value, "different value types (use value_cast)");
		                                                                      static_assert( std::is_same        <tag_type  , T>::value, "incompatible tags (use tag_cast)"      );
		                                                                      return std::abs(value - rhs.template get_scaled<scale_type>()) <= epsilon;
	                                                                      }

	/**
	 * @brief Check whether a floating point quantity is near zero
	 *
	 * Checks whether a floating point value is near zero. For integer types,
	 * this will test whether the value is zero.
	 *
	 * @param epsilon  Tolerance for comparison.
	 *
	 * @return true, if near zero
	 */
	constexpr bool is_near_zero(const ValueType epsilon = std::numeric_limits<value_type>::epsilon()) const
	                                                                      {return std::abs(value) <= epsilon;}

private:
	value_type                                        value;
};


/**
 * @{
 *
 * Calculate the types needed when multiplying/dividing quantities.
 */
template<typename Q1, typename Q2> using mul_quantity_t = quantity< mul_unit_t<typename Q1::unit_type, typename Q2::unit_type>
                                                                  , std::ratio_multiply<typename Q1::scale_type, typename Q2::scale_type>
                                                                  , decltype(typename Q1::value_type{}*typename Q2::value_type{}) >;
template<typename Q1, typename Q2> using div_quantity_t = quantity< div_unit_t<typename Q1::unit_type, typename Q2::unit_type>
                                                                  , std::ratio_divide<typename Q1::scale_type, typename Q2::scale_type>
                                                                  , decltype(typename Q1::value_type{}/typename Q2::value_type{}) >;
/** @} */


/**
 * @{
 *
 * Meta functions to get the type of a specifically scaled quantity.
 *
 * @note to_micro<to_kilo<meter>> will result in micro<meter>.
 */
template<typename Quantity> using no_scale = Quantity;

template<typename Quantity> using    atto  = typename Quantity::template rescale_by< atto_scaling>;
template<typename Quantity> using    femto = typename Quantity::template rescale_by<femto_scaling>;
template<typename Quantity> using    pico  = typename Quantity::template rescale_by< pico_scaling>;
template<typename Quantity> using    nano  = typename Quantity::template rescale_by< nano_scaling>;
template<typename Quantity> using    micro = typename Quantity::template rescale_by<micro_scaling>;
template<typename Quantity> using    milli = typename Quantity::template rescale_by<milli_scaling>;
template<typename Quantity> using    centi = typename Quantity::template rescale_by<centi_scaling>;
template<typename Quantity> using    deci  = typename Quantity::template rescale_by< deci_scaling>;
template<typename Quantity> using    deca  = typename Quantity::template rescale_by< deca_scaling>;
template<typename Quantity> using    hecto = typename Quantity::template rescale_by<hecto_scaling>;
template<typename Quantity> using    kilo  = typename Quantity::template rescale_by< kilo_scaling>;
template<typename Quantity> using    mega  = typename Quantity::template rescale_by< mega_scaling>;
template<typename Quantity> using    giga  = typename Quantity::template rescale_by< giga_scaling>;
template<typename Quantity> using    tera  = typename Quantity::template rescale_by< tera_scaling>;
template<typename Quantity> using    peta  = typename Quantity::template rescale_by< peta_scaling>;
template<typename Quantity> using    exa   = typename Quantity::template rescale_by<  exa_scaling>;

template<typename Quantity> using to_atto  = typename Quantity::template rescale_to< atto_scaling>;
template<typename Quantity> using to_femto = typename Quantity::template rescale_to<femto_scaling>;
template<typename Quantity> using to_pico  = typename Quantity::template rescale_to< pico_scaling>;
template<typename Quantity> using to_nano  = typename Quantity::template rescale_to< nano_scaling>;
template<typename Quantity> using to_micro = typename Quantity::template rescale_to<micro_scaling>;
template<typename Quantity> using to_milli = typename Quantity::template rescale_to<milli_scaling>;
template<typename Quantity> using to_centi = typename Quantity::template rescale_to<centi_scaling>;
template<typename Quantity> using to_deci  = typename Quantity::template rescale_to< deci_scaling>;
template<typename Quantity> using to_deca  = typename Quantity::template rescale_to< deca_scaling>;
template<typename Quantity> using to_hecto = typename Quantity::template rescale_to<hecto_scaling>;
template<typename Quantity> using to_kilo  = typename Quantity::template rescale_to< kilo_scaling>;
template<typename Quantity> using to_mega  = typename Quantity::template rescale_to< mega_scaling>;
template<typename Quantity> using to_giga  = typename Quantity::template rescale_to< giga_scaling>;
template<typename Quantity> using to_tera  = typename Quantity::template rescale_to< tera_scaling>;
template<typename Quantity> using to_peta  = typename Quantity::template rescale_to< peta_scaling>;
template<typename Quantity> using to_exa   = typename Quantity::template rescale_to<  exa_scaling>;
/** @} */

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
	static_assert( std::is_same<V1, V2>::value, "different value types (use value_cast)");
	static_assert( std::is_same<T1, T2>::value, "incompatible tags (use tag_cast)"      );
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
	static_assert( std::is_same<V1, V2>::value, "different value types (use value_cast)");
	static_assert( std::is_same<T1, T2>::value, "incompatible tags (use tag_cast)");
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
	static_assert( std::is_same<V1, V2>::value, "different value types (use value_cast)");
	static_assert( std::is_same<T1, T2>::value, "incompatible tags (use tag_cast)"      );
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
constexpr auto tag_cast(const quantity<U,S,ValueType,T>& q)               {return quantity<U,S,ValueType,NewTag>{tag_cast(q)};}
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
