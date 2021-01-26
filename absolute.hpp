#ifndef UNLIB_ABSOLUTE_HPP
#define UNLIB_ABSOLUTE_HPP

#include <limits>
#include <type_traits>
#include <ratio>

#include <unlib/quantity.hpp>

namespace unlib {

/**
 * @brief relation type
 *
 * This defines the relation of related absolutes. It provides an offset for
 * how different, but related absolutes in relation to a basic absolute. (For
 * example, Celsius is set off from Kelvin by 273,15.)
 *
 * @tparam Offset  the offset as a rational number
 *
 * @note There is no definition of this type other than the one where Offset
 *       is a `std::ratio`.
 */
template<typename Offset = std::ratio<0,1>>
struct relation;

template<std::intmax_t Num, std::intmax_t Den>
struct relation<std::ratio<Num,Den>> {
	using offset_type = std::ratio<Num,Den>;
};

/**
 * @brief Wrapper around absolute to allow specific implicit conversions.
 *
 * This type can be passed to an absolute and initiates the appropriate
 * conversions between different underlying quantities.
 *
 * @tparam          Relation  absolute's relation tag
 * @tparam              Unit  quantity's unit
 * @tparam             Scale  quantity's scale
 * @tparam         ValueType  quantity's value_type
 * @tparam               Tag  quantity's tag
 * @tparam ImplicitValueType  allow conversions that change ValueType
 * @tparam     ImplicitScale  allow conversions that change the unit's scale
 * @tparam       ImplicitTag  allow conversions that change the unit's tag
 *
 */
template< typename Relation
        , typename Dimension, typename Scale, typename ValueType, typename Tag
        , bool ImplicitValueType, bool ImplicitScale, bool ImplicitTag >
class implicit_absolute_caster : private implicit_quantity_caster<Dimension,Scale,ValueType,Tag,ImplicitValueType,ImplicitScale,ImplicitTag> {
	using quantity_caster = implicit_quantity_caster<Dimension,Scale,ValueType,Tag,ImplicitValueType,ImplicitScale,ImplicitTag>;
public:
	explicit constexpr implicit_absolute_caster(const quantity<Dimension,Scale,ValueType,Tag>& q)
	                                                                    : quantity_caster{q} {}

	template<typename NewScale, typename NewRelation>
	quantity<Dimension,NewScale,ValueType,Tag> get_relative() const {
		const double old_relation = static_cast<double>(   Relation::offset_type::num) /    Relation::offset_type::den;
		const double new_relation = static_cast<double>(NewRelation::offset_type::num) / NewRelation::offset_type::den;
		const double new_rescaled = detail::rescale_value<NewScale,Scale>(this->get() + old_relation);
		return quantity<Dimension,NewScale,ValueType,Tag>{ static_cast<ValueType>(new_rescaled - new_relation) };
	}
};

/**
 * @brief absolute values
 *
 * This wraps a quantity and puts it into a relation to other absolutes.
 * An example where this is needed is for times (seconds passed since the
 * start of a specific epoch) or temperatures (temperature degrees in a
 * specific scale: K vs. C vs. F).
 *
 * In order to provide this, absolutes are tagged with a relation. While this
 * could be a simple tag type (meaning that, for example, times in different
 * epochs – e.g., system time and UTC, are fundamentally incompatible and not
 * in a simple relation to each other), they could also be of a @ref relation
 * type, specifying an offset from a base absolute. The latter will allow
 * relations to be casted using a @ref relation_cast, so that, for example,
 * Kelvin can be converted to Celsius and Fahrenheit.
 *
 * @tparam          Relation  specifies the relationship with other absolutes using
 *                            quantities of the same dimension
 * @tparam          Quantity  underlying quantity
 * @tparam HasNegativeValues  whether absolute allows negative values
 */
template< typename Relation
        , typename Quantity
        , bool HasNegativeValues = not std::is_unsigned<typename Quantity::value_type>::value >
class absolute {
public:
	using relation_type             = Relation;                           /**< the absolute's relation type  */
	using quantity_type             = Quantity;                           /**< the absolute's quantity type  */

	/**
	 * @{
	 * shortcuts for the quantity's nested types
	 */
	using dimension_type            = typename quantity_type::dimension_type;
	using     scale_type            = typename quantity_type::    scale_type;
	using     value_type            = typename quantity_type::    value_type;
	using       tag_type            = typename quantity_type::      tag_type;
	/** @} */

	/**
	 * @{
	 * shortcuts for the quantity's base unit exponents
	 */
	using               time_exponent =               time_exponent_t<dimension_type>;
	using               mass_exponent =               mass_exponent_t<dimension_type>;
	using             length_exponent =             length_exponent_t<dimension_type>;
	using            current_exponent =            current_exponent_t<dimension_type>;
	using        temperature_exponent =        temperature_exponent_t<dimension_type>;
	using   substance_amount_exponent =   substance_amount_exponent_t<dimension_type>;
	using luminous_intensity_exponent = luminous_intensity_exponent_t<dimension_type>;
	/** @} */

	constexpr absolute()                                      = default;
	constexpr absolute(const absolute& rhs)                   = default;

	/**
	 * @brief Create an absolute from a compatible quantity
	 *
	 * @note A quantity is compatible if quantity_type can be constructed from it
	 *
	 * @param rhs  quantity to copy from
	 */
	template<typename D, typename S, typename V, typename T>
	constexpr explicit absolute(quantity<D,S,V,T> rhs)                  : relative(rhs) {}

	/**
	 * @brief Create an absolute from a compatible value type
	 *
	 * @note A value type is compatible if quantity_type can be constructed from it
	 *
	 * @param rhs  value type to copy from
	 */
	template<typename ValueType>
	constexpr explicit absolute(const ValueType v)                      : relative(v) {}

	/**
	 * @brief Conversion constructor
	 *
	 * Create an absolute from a casted quantity
	 *
	 * @param rhs  casted quantity to convert from
	 */
	template<typename D, typename S, typename V, typename T, bool IV, bool IS, bool IT>
	constexpr explicit absolute(const implicit_quantity_caster<D,S,V,T,IV,IS,IT>& rhs)
	                                                                    : relative{rhs} {}

	/**
	 * @brief Conversion constructor
	 *
	 * Create an absolute from a compatible absolute
	 *
	 * @note Absolutes are compatible if their quantities can be constructed from each other
	 *
	 * @param rhs  quantity to convert from
	 */
	template<typename R, typename D, typename S, typename V, typename T, bool IV, bool IS, bool IT>
	constexpr absolute(const implicit_absolute_caster<R,D,S,V,T,IV,IS,IT>& rhs)
	                                                                    : relative{rhs.template get_relative<scale_type, Relation>()} {}

	constexpr absolute& operator=(const absolute& rhs)        = default;

	/**
	 * @brief assign a casted absolute
	 *
	 * assign from a casted absolute
	 *
	 * @param rhs  quantity to convert from
	 */
	template<typename R, typename D, typename S, typename V, typename T, bool IV, bool IS, bool IT>
	constexpr absolute& operator=(const implicit_absolute_caster<R,D,S,V,T,IV,IS,IT>& rhs)
	                                                                      {relative = rhs.template get_relative<scale_type, Relation>(); return *this;}

	/**
	 * @brief get relative value
	 *
	 * @return absolute's relative value
	 */
	constexpr quantity_type relative_value() const                        {return relative;};

	/**
	 * @{
	 *
	 * @brief arithmetic operations
	 *
	 * Add/subtract relative quantities to/from this absolute
	 *
	 * @param rhs  quantities to add/subtract
	 *
	 * @return *this
	 */
	template<typename D, typename S, typename V, typename T>
	constexpr absolute& operator+=(quantity<D,S,V,T> rhs)                 {relative += rhs; return *this;}
	template<typename D, typename S, typename V, typename T>
	constexpr absolute& operator-=(quantity<D,S,V,T> rhs)                 {relative -= rhs; return *this;}
	/** @} */

	/**
	 * @brief unary negation
	 *
	 * @returns -(*this)
	 */
	constexpr absolute operator-() const                                  {
		                                                                      static_assert(std::is_signed<value_type>::value, "unary minus on unsigned");
		                                                                      return absolute{ -relative_value() };
	                                                                      }

	/**
	 * @{
	 * @brief Increment and decrement
	 *
	 * @return *this for prefix versions, copy of *this for postfix versions
	 */
	template<typename A=absolute> constexpr std::enable_if_t<std::numeric_limits<value_type>::is_integer, A>& operator++()    {++relative; return *this;}
	template<typename A=absolute> constexpr std::enable_if_t<std::numeric_limits<value_type>::is_integer, A>& operator--()    {--relative; return *this;}
	template<typename A=absolute> constexpr std::enable_if_t<std::numeric_limits<value_type>::is_integer, A>  operator++(int) {return absolute(relative++);}
	template<typename A=absolute> constexpr std::enable_if_t<std::numeric_limits<value_type>::is_integer, A>  operator--(int) {return absolute(relative--);}
	/** @} */

private:
	quantity_type                                     relative;
};

/**
 * @{
 *
 * @brief absolute comparison
 *
 * @param lhs, rhs  operands
 * @tparam A  absolute tag type
 * @tparam Q  quantity type
 *
 * @result comparison result
 */
template<typename R, typename Q, bool N> bool operator==(absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() == rhs.relative_value();}
template<typename R, typename Q, bool N> bool operator!=(absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() != rhs.relative_value();}
template<typename R, typename Q, bool N> bool operator< (absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() <  rhs.relative_value();}
template<typename R, typename Q, bool N> bool operator> (absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() >  rhs.relative_value();}
template<typename R, typename Q, bool N> bool operator<=(absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() <= rhs.relative_value();}
template<typename R, typename Q, bool N> bool operator>=(absolute<R,Q,N> lhs, absolute<R,Q,N> rhs) {return lhs.relative_value() >= rhs.relative_value();}
/** @} */

/**
 * @{
 *
 * @brief absolute arithmetic
 *
 * This allows the addition and subtraction of quantities to, or from, time
 * points (resulting in a new absolute), as well as the subtraction of
 * absolutes (resulting in a quantity).
 *
 * @param lhs, rhs  operands
 *
 * @result absolute or quantity
 */
template<typename Relation, typename Q1, bool N, typename Q2>
absolute<Relation,Q1> operator+(absolute<Relation,Q1,N> lhs, Q2 rhs)      {return lhs += rhs;}

template<typename Relation, typename Q1, typename Q2, bool N>
absolute<Relation,Q1> operator+(Q1 lhs, absolute<Relation,Q2,N> rhs)      {return rhs += lhs;}

template<typename Relation, typename Q1, bool N, typename Q2>
absolute<Relation,Q1> operator-(absolute<Relation,Q1,N> lhs, Q2 rhs)      {return lhs -= rhs;}

template<typename Relation, typename D, typename S1, typename S2, typename V, typename T, bool N>
quantity<D,S1,V,T> operator-(absolute<Relation,quantity<D,S1,V,T>,N> lhs, absolute<Relation,quantity<D,S2,V,T>,N> rhs )
                                                                          {return lhs.relative_value() -= rhs.relative_value();}
/** @} */

/**
 * @{
 * @brief Cast between related absolutes
 *
 * Performs casts between related absolutes, e.g., Celsius to Kelvin. This
 * only works if the absolutes involved use the same quantity types and are
 * related by using a @ref relation type with the same tag.
 *
 * @param            a  absolute to cast
 *
 * @tparam NewRelation  (optional) absolute to cast to
 *
 * @return a converted to new absolute
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * fahrenheit f{                                         300_K }; // error
 * fahrenheit f{relation_cast<fahrenheit::relation_type>(300_K)}; // ok
 * fahrenheit f{relation_cast                           (300_K)}; // ok
 */
template<typename R, typename D, typename S, typename V, typename T, bool N>
constexpr implicit_absolute_caster<R,D,S,V,T,false,true,true> relation_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return implicit_absolute_caster<R,D,S,V,T,false,true,true>{a.relative_value()};}
template<typename RelatedAbsolute, typename R, typename D, typename S, typename V, typename T, bool N>
constexpr RelatedAbsolute relation_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return RelatedAbsolute{relation_cast(a)};}
/** @} */


/**
 * @{
 * @brief cast between absolutes of different ValueTypes
 *
 * Performs casts between absolutes of compatible dimensions with different
 * cakue types.
 *
 * @param             a  absolute to cast
 *
 * @tparam NewValueType  (optional) the value type to cast to
 *
 * @return a with same dimension, but static_cast<>'ed value
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * absolute<system_clock, milli<seconds<std::int16_t>>> value{                         1_ms }; // error, milli<seconds<unsigned long long>>
 * absolute<system_clock, milli<seconds<std::int16_t>>> value{value_cast<std::int16_t>(1_ms)}; // OK
 * absolute<system_clock, milli<seconds<std::int16_t>>> value{value_cast              (1_ms)}; // OK
 *
 */
template<typename R, typename V, typename D, typename S, typename T, bool N>
constexpr implicit_absolute_caster<R,D,S,V,T,true,false,false> value_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return implicit_absolute_caster<R,D,S,V,T,true,false,false>{a.relative_value()};}
template<typename NewValueType, typename R, typename D, typename S, typename T, typename V, bool N>
constexpr absolute<R, quantity<D,S,NewValueType,T>> value_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return absolute<R,quantity<D,S,NewValueType,T>>{value_cast(a)};}
/**@} */

/**
 * @{
 * @brief Cast between differently scaled absolutes
 *
 * Performs casts between related, compatible absolutes with different scales,
 * e.g. from a time point in milliseconds to a time point seconds.
 *
 * @param         a  absolute to cast
 * @tparam NewScale  (optional) scale to cast to, e.g. milli_scaling
 * @return a with rescaled value and changed scale
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * absolute<system_clock,seconds<unsigned long long>> value{                          1_ms }; // error, milli<seconds<unsigned long long>>
 * absolute<system_clock,seconds<unsigned long long>> value{scale_cast<milli_scaling>(1_ms)}; // OK
 * absolute<system_clock,seconds<unsigned long long>> value{scale_cast               (1_ms)}; // OK
 */
template<typename R, typename S, typename D, typename V, typename T, bool N>
constexpr implicit_absolute_caster<R,D,S,V,T,false,true,false> scale_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return implicit_absolute_caster<R,D,S,V,T,false,true,false>{a.relative_value()};}
template<typename NewScale, typename R, typename D, typename S, typename V, typename T, bool N>
constexpr typename absolute<R,quantity<D,S,V,T>>::template scale_by<NewScale> scale_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return typename absolute<R,quantity<D,S,V,T>>::template scale_by<NewScale>{scale_cast(a)};}
/** @} */

/**
 * @{
 * @brief Cast between quantities with different tags
 *
 * Performs casts between quantities of compatible dimensions with different
 * tags.
 *
 * @param       a  quantity to cast
 *
 * @tparam NewTag  (optional)  tag to cast to
 *
 * @return a with changed tag
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * kilo<voltampere<double>> value =                                1_kVAr ; // error, kilo<var<double>>
 * kilo<voltampere<double>> value = tag_cast<voltampere::tag_type>(1_kVAr); // OK
 * kilo<voltampere<double>> value = tag_cast                      (1_kVAr); // OK
 */
template<typename R, typename V, typename D, typename S, typename T, bool N>
constexpr implicit_absolute_caster<R,D,S,V,T,false,false,true> tag_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return implicit_absolute_caster<R,D,S,V,T,false,false,true>{a};}
template<typename NewTag, typename R, typename V, typename D, typename S, typename T, bool N>
constexpr absolute<R,quantity<D,S,V,NewTag>> tag_cast(const absolute<R,quantity<D,S,V,T>,N>& a)
                                                                          {return absolute<R,quantity<D,S,V,NewTag>>{tag_cast(a)};}
/** @} */


/**
 * @brief Cast between between absolutes with different scale, tag and/or value_type
 *
 * Performs casts between absolutes of compatible units with different
 * scales, tags and/or value_types.
 *
 * @param            q  absolute to cast
 * @tparam Newabsolute  absolute to cast to
 * @return q converted to Newabsolute
 *
 * @note Leaving off the optional template parameter will return an
 *       intermediate object on which the conversion will be implicitly
 *       allowed. Pass that to the quantity constructor to invoke the
 *       conversion. As with all implicit conversions, use this with care.
 *
 * @code
 * absolute<si::kilovoltampere, no_scaling, float> value =                                   1 * constants::kilovar ; // error
 *                                            auto value = absolute_cast<si::kilovoltampere>(1 * constants::kilovar); // ok
 * absolute<si::kilovoltampere, no_scaling, float> value = absolute_cast                    (1 * constants::kilovar); // ok
 */
template<typename R, typename D, typename S, typename V, typename T, bool N>
constexpr implicit_absolute_caster<R,D,S,V,T,true,true,true> absolute_cast(const absolute<R,quantity<D,S,V,T>,N>& q)
                                                                          {return implicit_absolute_caster<R,D,S,V,T,true,true,true>{q.relative_value()};}
template<typename NewAbsolute, typename R, typename D, typename S, typename V, typename T, bool N>
constexpr NewAbsolute absolute_cast(const absolute<R,quantity<D,S,V,T>,N>& q)
                                                                          {return NewAbsolute{absolute_cast(q)};}
/** @} */
}

#endif /* UNLIB_ABSOLUTE_HPP */
