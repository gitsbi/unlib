#ifndef UNLIB_COMMON_HPP
#define UNLIB_COMMON_HPP

/*
 * common.hpp
 *
 * Copyright sbi http://stackoverflow.com/users/140719
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *     http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cinttypes>

#include <unlib/quantity.hpp>

namespace unlib {

/** @{
 * @brief some common physical units derived from the seven base units
 *
 * This defines a few derived physical units. They can be used to declare
 * templates which refer to a quantity of some unit at any scale.
 *
 * @code
 * template<typename Scale>
 * void f(unlib::quantity<unlib::velocity, Scale, double, void>);
 *
 */
using            frequency = reciprocal_unit_t< time >;
using             velocity =        div_unit_t< length , time >;
using                 area =     square_unit_t< length >;
using               volume =       cube_unit_t< length >;
using             pressure =        div_unit_t< mass   , mul_unit_t<length, square_unit_t<time>> >;

using              voltage =        div_unit_t< mul_unit_t<mass   , square_unit_t<length>>
                                              , mul_unit_t<current,   cube_unit_t<time  >> >;
using                power =        mul_unit_t< current, voltage >;
using               energy =        mul_unit_t< power  , time    >;
using      electric_charge =        mul_unit_t< current, time    >;
using           resistance =        div_unit_t< voltage, current >;
using volumetric_flow_rate =        div_unit_t< volume , time    >;
/** @} */

#if !defined(UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE)
#   define UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE  long
#endif
#if !defined(UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE)
#   define UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE  double
#endif

namespace literals {
using integer_value_type = UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE; /**< value type of quantity return by integer literal operator */
using floatpt_value_type = UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE; /**< value type of quantity return by float   literal operator */
}

/** @{
 *
 * Macros to define literal operators for units and for their prefixed versions
 *
 * @param       Qty_  Quantity (e.g., meter)
 * @param        Sc_  Scale (e.g., kilo)
 * @param UnitShort_  Unit's short name (e.g., m)
 * @param       OpT_  Operand type for literal operator (e.g., unsigned long long or long double)
 * @param         R_  Return type of literal operator (e.g., long long or double)
 */
/** defines a literal operator; do not call this directly */
#define UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,OpT_, R_)       inline constexpr auto operator""_##UnitShort_(OpT_ v) noexcept {return Sc_<Qty_<R_>>{static_cast<R_>(v)};}
/** defines a set of literal operators for a unit return double and long long */
#define UNLIB_DEFINE_SCALED_LITERAL(Qty_,Sc_,UnitShort_)                  namespace literals {                                                                      \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,long double       , floatpt_value_type) \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,unsigned long long, integer_value_type) \
                                                                          }
/** define the literal operators for all the micro unit prefixes (atto-deci) */
#define UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL(Qty_,UnitShort_)       UNLIB_DEFINE_SCALED_LITERAL(Qty_,atto , a##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,femto, f##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,pico , p##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,nano , n##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,micro, u##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,milli, m##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,centi, c##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,deci , d##UnitShort_)
/** define the literal operators for all the macro unit prefixes (deca-tera) */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(Qty_,UnitShort_)       UNLIB_DEFINE_SCALED_LITERAL(Qty_,deca ,dk##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,hecto, h##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,kilo , k##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,mega , M##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,giga , G##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,tera , T##UnitShort_)
                                                                        //UNLIB_DEFINE_SCALED_LITERAL(Qty_,peta , P##UnitShort_)
                                                                        //UNLIB_DEFINE_SCALED_LITERAL(Qty_,exa  , E##UnitShort_)
/** define the literal operators for all unit prefixes (atto-tera) */
#define UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(Qty_,UnitShort_)            UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL(Qty_,UnitShort_) \
                                                                          UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(Qty_,UnitShort_)
/** define the literal operators for a physical unit (nor prefixes) */
#define UNLIB_DEFINE_LITERAL(Qty_,UnitShort_)                             UNLIB_DEFINE_SCALED_LITERAL(Qty_,no_scale,UnitShort_)
/** @} */


/**
 * @{
 *
 * time quantities
 *
 * @tparam V  value type
 */
template<typename V> using   second = quantity<time, no_scaling, V>;                                             UNLIB_DEFINE_LITERAL(second, s   )            UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(second, s)
template<typename V> using   minute = minute_scale<second<V>>;                                                   UNLIB_DEFINE_LITERAL(minute, min )
template<typename V> using     hour =   hour_scale<second<V>>;                                                   UNLIB_DEFINE_LITERAL(  hour, h   )
template<typename V> using      day =    day_scale<second<V>>;                                                   UNLIB_DEFINE_LITERAL(   day, d   )
template<typename V> using     week =   week_scale<second<V>>;                                                   UNLIB_DEFINE_LITERAL(  week, week)
/** @} */

/**
 * @{
 *
 * mass quantities
 *
 * @tparam V  value type
 */
template<typename V> using     gram = quantity<mass, no_scaling, V>;                                             UNLIB_DEFINE_LITERAL(gram, g)                 UNLIB_DEFINE_METRIC_PREFIXED_LITERALS     (gram, g)
template<typename V> using      ton = to_mega<gram<V>>;                                                          UNLIB_DEFINE_LITERAL( ton, t)                 UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(ton, t)
/** @} */

/**
 * @{
 *
 * length quantities
 *
 * @tparam V  value type
 */
template<typename V> using    meter = quantity<length, no_scaling, V>;                                           UNLIB_DEFINE_LITERAL(meter, m)                UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(meter, m)
/** @} */

/**
 * @{
 *
 * area quantities
 *
 * @tparam V  value type
 */
template<typename V> using  square_millimeter = quantity<area, micro_scaling, V>;                                UNLIB_DEFINE_LITERAL(square_millimeter, mm2)
template<typename V> using  square_centimeter = quantity<area, std::ratio<1,10000>, V>;                          UNLIB_DEFINE_LITERAL(square_centimeter, cm2)
template<typename V> using       square_meter = quantity<area,    no_scaling, V>;                                UNLIB_DEFINE_LITERAL(     square_meter,  m2)
template<typename V> using   square_kilometer = quantity<area,  mega_scaling, V>;                                UNLIB_DEFINE_LITERAL( square_kilometer, km2)
template<typename V> using                are = quantity<area, std::ratio<100,1>, V>;
template<typename V> using            hectare = quantity<area, std::ratio<10000,1>, V>;                          UNLIB_DEFINE_LITERAL(          hectare,  ha)
/** @} */

/**
 * @{
 *
 * volume quantities
 *
 * @tparam V  value type
 */
template<typename V> using   cubic_millimeter = quantity<volume,  nano_scaling, V>;                              UNLIB_DEFINE_LITERAL( cubic_millimeter, mm3)
template<typename V> using   cubic_centimeter = quantity<volume, micro_scaling, V>;                              UNLIB_DEFINE_LITERAL( cubic_centimeter, cm3)
template<typename V> using   cubic_meter      = quantity<volume,    no_scaling, V>;                              UNLIB_DEFINE_LITERAL(      cubic_meter,  m3)
template<typename V> using   cubic_kilometer  = quantity<volume,  giga_scaling, V>;                              UNLIB_DEFINE_LITERAL(  cubic_kilometer, km3)
template<typename V> using         milliliter = cubic_centimeter<V>;                                             UNLIB_DEFINE_LITERAL(       milliliter,  ml)
template<typename V> using              liter = quantity<volume, milli_scaling, V>;                              UNLIB_DEFINE_LITERAL(            liter,   l)
/** @} */

/**
 * @{
 *
 * temperature quantities
 *
 * @tparam V  value type
 */
template<typename V> using  degree_kelvin     = quantity<temperature, no_scaling, V>;
template<typename V> using  degree_celsius    = quantity<temperature, no_scaling, V>;
template<typename V> using  degree_fahrenheit = quantity<temperature, std::ratio<5,9>, V>;
/** @} */

/**
 * @{
 *
 * frequency quantities
 *
 * @tparam V  value type
 */
template<typename V> using    hertz = quantity<frequency, no_scaling, V>;                                        UNLIB_DEFINE_LITERAL(hertz, Hz)               UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(hertz, Hz)
/** @} */

/**
 * @{
 *
 * electrical quantities
 *
 * @tparam V  value type
 */
template<typename V> using     ampere = quantity<current   , no_scaling, V>;                                     UNLIB_DEFINE_LITERAL(ampere, A)               UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere, A)
template<typename V> using       volt = quantity<voltage   , no_scaling, V>;                                     UNLIB_DEFINE_LITERAL(  volt, V)               UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(volt  , V)
template<typename V> using        ohm = quantity<resistance, no_scaling, V>;                                     UNLIB_DEFINE_LITERAL(   ohm, O)               UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ohm   , O)
/** @} */

/**
 * @{
 *
 * power quantities
 *
 * @tparam V  value type
 */
using reactive_power_tag = tag_t<struct reactive_power_tag_id>;
using apparent_power_tag = tag_t<struct apparent_power_tag_id>;
template<typename V> using       watt = quantity<power, no_scaling, V>;                                          UNLIB_DEFINE_LITERAL(      watt, W  )         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(watt      , W  )
template<typename V> using        var = typename watt<V>::template retag<reactive_power_tag>;                    UNLIB_DEFINE_LITERAL(       var, VAr)         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(var       , VAr)
template<typename V> using voltampere = typename watt<V>::template retag<apparent_power_tag>;                    UNLIB_DEFINE_LITERAL(voltampere, VA )         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(voltampere, VA )
/** @} */

/**
 * @{
 *
 * energy quantities
 *
 * @tparam V  value type
 */
using joule_tag = tag_t<struct jule_tag_id>;
template<typename V> using             joule = quantity<energy, no_scaling, V, joule_tag>;                       UNLIB_DEFINE_LITERAL(            joule, J   ) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(      joule      , J   )

template<typename V> using       watt_second = typename joule<V>::untag;                                         UNLIB_DEFINE_LITERAL(      watt_second, Ws  ) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(      watt_second, Ws  )
template<typename V> using       watt_hour   = scale_to_t<hour_scaling,       watt_second<V>>;                   UNLIB_DEFINE_LITERAL(      watt_hour  , Wh  ) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(      watt_hour  , Wh  )

template<typename V> using        var_second = typename joule<V>::template retag<reactive_power_tag>;            UNLIB_DEFINE_LITERAL(       var_second, VArs) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(       var_second, VArs)
template<typename V> using        var_hour   = scale_to_t<hour_scaling,        var_second<V>>;                   UNLIB_DEFINE_LITERAL(       var_hour  , VArh) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(       var_hour  , VArh)

template<typename V> using voltampere_second = typename joule<V>::template retag<apparent_power_tag>;            UNLIB_DEFINE_LITERAL(voltampere_second, VAs ) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(voltampere_second, VAs )
template<typename V> using voltampere_hour   = scale_to_t<hour_scaling, voltampere_second<V>>;                   UNLIB_DEFINE_LITERAL(voltampere_hour  , VAh ) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(voltampere_hour  , VAh )
/** @} */

/**
 * @{
 *
 * electric charge quantities
 *
 * @tparam V  value type
 */
template<typename V> using ampere_second = quantity<electric_charge, no_scaling, V>;                             UNLIB_DEFINE_LITERAL(ampere_second, As)       UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere_second, As)
template<typename V> using ampere_hour   = typename ampere_second<V>::template rescale_by<hour_scaling>;         UNLIB_DEFINE_LITERAL(ampere_hour  , Ah)       UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere_hour  , Ah)
/** @} */

/**
 * @{
 *
 * pressure quantities
 *
 * @tparam V  value type
 *
 * @note Sadly, `pascal` is a keyword/macro on Windows; hence `pascal_`.
 */
template<typename V> using pascal_ = quantity<pressure, no_scaling   , V>;                                       UNLIB_DEFINE_LITERAL(pascal_, Pa )            UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(pascal_, Pa )
template<typename V> using     bar = quantity<pressure, scale_t<100000>, V>;                                     UNLIB_DEFINE_LITERAL(bar    , bar)            UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(bar    , bar)
/** @} */

/**
 * @{
 *
 * velocity quantities
 *
 * @tparam V  value type
 */
template<typename V> using     meter_per_second = div_quantity_t<meter<V>, second<V>>;                           UNLIB_DEFINE_LITERAL(    meter_per_second, m_per_s )
template<typename V> using kilometer_per_hour   = div_quantity_t<to_kilo<meter<V>>, hour<V>>;                    UNLIB_DEFINE_LITERAL(kilometer_per_hour  , km_per_h)
/** @} */

/**
 * @{
 *
 * volumetric flow rate
 *
 * @tparam V  value type
 */
template<typename V> using       liter_per_hour = div_quantity_t<liter<V>, hour<V>>;                             UNLIB_DEFINE_LITERAL(liter_per_hour, l_per_h)
/** @} */

/**
 * @{
 *
 * @brief scalar quantities
 *
 * Scalars are quantities with a dimensionless unit. The offer little
 * advantage over a plain value type when untagged, so they should always
 * have a tag.
 *
 * @tparam V  value type
 * @tparam T  tag
 * @tparam S  scale
 */
template<typename V, typename T, typename S=scale_t<1>> using scalar   = quantity<dimensionless, S, V, T>;
template<typename V, typename S=scale_t<1>>             using fraction = scalar<V, no_tag, S>;
template<typename V>                                    using percent  = typename fraction<V>::template rescale_by<centi_scaling>;
template<typename V>                                    using permill  = typename fraction<V>::template rescale_by<milli_scaling>;
/** @} */
}

#endif /* UNLIB_COMMON_HPP */
