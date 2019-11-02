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

#include <unlib/quantity.hpp>

namespace unlib {

/**
 * @{
 * @brief the units representing the seven basic SI units
 */
using             time = unit< exponent<1>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0> >;
using             mass = unit< exponent<0>, exponent<1>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0> >;
using           length = unit< exponent<0>, exponent<0>, exponent<1>, exponent<0>, exponent<0>, exponent<0>, exponent<0> >;
using          current = unit< exponent<0>, exponent<0>, exponent<0>, exponent<1>, exponent<0>, exponent<0>, exponent<0> >;
using       luminosity = unit< exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<1>, exponent<0>, exponent<0> >;
using      temperature = unit< exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<1>, exponent<0> >;
using substance_amount = unit< exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<0>, exponent<1> >;
/** @} */

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
using        frequency = reciprocal_unit_t<time>;

using         velocity = div_unit_t<length, time>;

using          voltage = div_unit_t< mul_unit_t<mass   , square_unit_t<length>>
                                   , mul_unit_t<current,   cube_unit_t<time  >> >;

using            power = mul_unit_t<current,voltage>;

using           energy = mul_unit_t<power,time>;

using  electric_charge = mul_unit_t<current,time>;

using       resistance = div_unit_t<voltage,current>;

using         pressure = div_unit_t< mass
                                   , mul_unit_t< length
                                               , square_unit_t<time> > >;
/** @} */

#define UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,OpT_, R_)       inline constexpr auto operator""_##UnitShort_(OpT_ v) noexcept {return Sc_<Qty_<R_>>{static_cast<R_>(v)};}
#define UNLIB_DEFINE_SCALED_LITERAL(Qty_,Sc_,UnitShort_)                  namespace literals {                                                             \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,long double       , double   ) \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,unsigned long long, long long) \
                                                                          }
#define UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL(Qty_,UnitShort_)       UNLIB_DEFINE_SCALED_LITERAL(Qty_,atto , a##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,femto, f##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,pico , p##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,nano , n##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,micro, u##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,milli, m##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,centi, c##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,deci , d##UnitShort_)
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(Qty_,UnitShort_)       UNLIB_DEFINE_SCALED_LITERAL(Qty_,deca ,dk##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,hecto, h##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,kilo , k##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,mega , M##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,giga , G##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,tera , T##UnitShort_)
                                                                        //UNLIB_DEFINE_SCALED_LITERAL(Qty_,peta , P##UnitShort_)
                                                                        //UNLIB_DEFINE_SCALED_LITERAL(Qty_,exa  , E##UnitShort_)
#define UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(Qty_,UnitShort_)            UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL(Qty_,UnitShort_) \
                                                                          UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(Qty_,UnitShort_)
#define UNLIB_DEFINE_LITERAL(Qty_,UnitShort_)                             UNLIB_DEFINE_SCALED_LITERAL(Qty_,no_scale,UnitShort_)


/** @{
 * time quantities
 */
template<typename V> using second = quantity<time, no_scaling, V>;                                       UNLIB_DEFINE_LITERAL(second, s   )      UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(second, s)
template<typename V> using minute = typename second<V>::template rescale_by<minute_scaling>;             UNLIB_DEFINE_LITERAL(minute, min )
template<typename V> using   hour = typename second<V>::template rescale_by<  hour_scaling>;             UNLIB_DEFINE_LITERAL(  hour, h   )
template<typename V> using    day = typename second<V>::template rescale_by<   day_scaling>;             UNLIB_DEFINE_LITERAL(   day, d   )
template<typename V> using   week = typename second<V>::template rescale_by<  week_scaling>;             UNLIB_DEFINE_LITERAL(  week, week)
/** @} */

/** @{
 * mass quantities
 */
template<typename V> using     gram = quantity<mass, no_scaling, V>;                                     UNLIB_DEFINE_LITERAL(gram, g)           UNLIB_DEFINE_METRIC_PREFIXED_LITERALS     (gram, g)
template<typename V> using      ton = to_mega<gram<V>>;                                                  UNLIB_DEFINE_LITERAL( ton, t)           UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL(ton, t)
/** @} */

/** @{
 * length quantities
 */
template<typename V> using    meter = quantity<length, no_scaling, V>;                                   UNLIB_DEFINE_LITERAL(meter, m)          UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(meter, m)
/** @} */

/** @{
 * temperature quantities
 */
template<typename V> using  degree_kelvin     = quantity<temperature, no_scaling, V>;
template<typename V> using  degree_celsius    = quantity<temperature, no_scaling, V>;
template<typename V> using  degree_fahrenheit = quantity<temperature, std::ratio<5,9>, V>;
/** @} */

/** @{
 * frequency quantities
 */
template<typename V> using    hertz = quantity<frequency, no_scaling, V>;                                UNLIB_DEFINE_LITERAL(hertz, Hz)         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(hertz, Hz)
/** @} */

/** @{
 * electrical quantities
 */
template<typename V> using     ampere = quantity<current   , no_scaling, V>;                             UNLIB_DEFINE_LITERAL(ampere, A)         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere, A)
template<typename V> using       volt = quantity<voltage   , no_scaling, V>;                             UNLIB_DEFINE_LITERAL(  volt, V)         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(volt  , V)
template<typename V> using        ohm = quantity<resistance, no_scaling, V>;                             UNLIB_DEFINE_LITERAL(   ohm, O)         UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ohm   , O)
/** @} */

/** @{
 * power quantities
 */
template<typename V> using       watt = quantity<power, no_scaling, V>;                                  UNLIB_DEFINE_LITERAL(      watt, W  )   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(watt      , W  )
template<typename V> using        var = typename watt<V>::template retag<struct reactive_power_tag>;     UNLIB_DEFINE_LITERAL(       var, VAr)   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(var       , VAr)
template<typename V> using voltampere = typename watt<V>::template retag<struct apparent_power_tag>;     UNLIB_DEFINE_LITERAL(voltampere, VA )   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(voltampere, VA )
/** @} */

/** @{
 * energy quantities
 */
template<typename V> using watt_second = quantity<energy, no_scaling, V>;                                UNLIB_DEFINE_LITERAL(watt_second, Ws)   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(watt_second, Ws)
template<typename V> using       joule = typename watt_second<V>::template retag<struct joule_tag>;      UNLIB_DEFINE_LITERAL(      joule, J )   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(joule      , J )
template<typename V> using   watt_hour = typename watt_second<V>::template rescale_by<hour_scaling>;     UNLIB_DEFINE_LITERAL(watt_hour  , Wh)   UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(watt_hour  , Wh)
/** @} */

/** @{
 * electric charge quantities
 */
template<typename V> using ampere_second = quantity<electric_charge, no_scaling, V>;                     UNLIB_DEFINE_LITERAL(ampere_second, As) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere_second, As)
template<typename V> using ampere_hour   = typename ampere_second<V>::template rescale_by<hour_scaling>; UNLIB_DEFINE_LITERAL(ampere_hour  , Ah) UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(ampere_hour  , Ah)
/** @} */

/** @{
 * pressure quantities
 *
 * @note Sadly, `pascal` is a keyword/macro on Windows; hence `pascal_`.
 */
template<typename V> using pascal_ = quantity<pressure, no_scaling   , V>;                               UNLIB_DEFINE_LITERAL(pascal_, Pa )      UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(pascal_, Pa )
template<typename V> using     bar = quantity<pressure, scale<100000>, V>;                               UNLIB_DEFINE_LITERAL(bar    , bar)      UNLIB_DEFINE_METRIC_PREFIXED_LITERALS(bar    , bar)
/** @} */

/** @{
 * velocity quantities
 */
template<typename V> using     meter_per_second = div_quantity_t< meter<V>, second<V> >;                 UNLIB_DEFINE_LITERAL(    meter_per_second, m_per_s )
template<typename V> using kilometer_per_hour   = div_quantity_t< to_kilo<meter<V>>, hour<V> >;          UNLIB_DEFINE_LITERAL(kilometer_per_hour  , km_per_h)
/** @} */

}

#endif /* UNLIB_COMMON_HPP */
