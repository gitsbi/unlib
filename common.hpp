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

/**
 * @brief namespace for unit literals
 *
 * All unit literal operators are defined within this namespace. In order to
 * use the literal operators, you need a using directive for this namespace.
 *
 * Note that in this namespace the two possible value types of the quantities
 * returned by the literals are defined as well. `integer_value_type` is the
 * value type of quantities defined using an integer literal,
 * `floatpt_value_type` is the one for floating point literals. If you want
 * to use literals (and who wouldn't?), it's easiest to define your quantities
 * so that those are their value types.
 *
 * These types default to `long` and `double`, but you can change them by
 * defining `UNLIB_INTEGER_VALUE_TYPE` and/or `UNLIB_FLOATPT_VALUE_TYPE` as
 * the type of your choice at compile time. (Make sure they are defined the
 * same in all translation units.)
 *
@code
using namespace unlib::literals
unlib::kilo<unlib::watt_hours<floatpt_value_type>>> energy = 42._kWh;
@endcode
 *
 */
namespace literals {

#if !defined(UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE)
#   define UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE  long
#endif
#if !defined(UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE)
#   define UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE  double
#endif

using integer_value_type = UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE; /**< value type of quantity return by integer literal operator */
using floatpt_value_type = UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE; /**< value type of quantity return by float   literal operator */

}

/**
 * @{
 *
 * quantities of basic units
 *
 * @tparam V  value type
 *
 * @note For whatever reason, in the SI system, the base unit of mass is
 *       kilogram, not gram.
 */
template<typename V> using                   second = quantity<time              , no_scaling, V>;
template<typename V> using                 kilogram = quantity<mass              , no_scaling, V>;
template<typename V> using                    meter = quantity<length            , no_scaling, V>;
template<typename V> using                   ampere = quantity<current           , no_scaling, V>;
template<typename V> using            degree_kelvin = quantity<temperature       , no_scaling, V>;
template<typename V> using                      mol = quantity<substance_amount  , no_scaling, V>;
template<typename V> using                  candela = quantity<luminous_intensity, no_scaling, V>;
/** @} */


/**
 * @{
 *
 * some derived units and their quantities
 *
 * @tparam V  value type
 */

/** @{ our Babylonian scaling of time */
template<typename V> using                  minute  = minute_scale<second<V>>;
template<typename V> using                    hour  =   hour_scale<second<V>>;
template<typename V> using                     day  =    day_scale<second<V>>;
template<typename V> using                    week  =   week_scale<second<V>>;
/** @} */

template<typename V> using                    gram  = milli<kilogram<V>>;
template<typename V> using                     ton  = kilo<kilogram<V>>; /**< @note Instead of megagram, ton is used */

/** @{ humans insist on using non-Kelvin temperature scales ¯\_(ツ)_/¯ */
template<typename V> using       degree_celsius     = quantity<temperature, no_scaling     , V>;
template<typename V> using       degree_fahrenheit  = quantity<temperature, std::ratio<5,9>, V>;
/** @} */

/** frequency */
template<typename V> using                   hertz  = reciprocal_quantity_t<second<V>>;

/** @{ area */
template<typename V> using       square_millimeter  = square_quantity_t<milli<meter<V>>>;
template<typename V> using       square_centimeter  = square_quantity_t<centi<meter<V>>>;
template<typename V> using       square_decimeter   = square_quantity_t< deci<meter<V>>>;
template<typename V> using       square_meter       = square_quantity_t<      meter<V> >;
template<typename V> using       square_kilometer   = square_quantity_t< kilo<meter<V>>>;
template<typename V> using                     are  = square_quantity_t< deca<meter<V>>>;
template<typename V> using                 hectare  = square_quantity_t<hecto<meter<V>>>;
/** @} */

/** @{ volume */
template<typename V> using        cubic_millimeter  = cube_quantity_t<milli<meter<V>>>;
template<typename V> using        cubic_centimeter  = cube_quantity_t<centi<meter<V>>>;
template<typename V> using        cubic_decimeter   = cube_quantity_t< deci<meter<V>>>;
template<typename V> using        cubic_meter       = cube_quantity_t<      meter<V> >;
template<typename V> using        cubic_kilometer   = cube_quantity_t< kilo<meter<V>>>;
template<typename V> using              milliliter  = cubic_centimeter<V>;
template<typename V> using                   liter  = cubic_decimeter<V>;
/** @} */

/** @{ velocity */
template<typename V> using        meter_per_second  = div_quantity_t<meter<V>, second<V>>;
template<typename V> using    kilometer_per_hour    = div_quantity_t<to_kilo<meter<V>>, hour<V>>;
/** @} */

/** @{ acceleration */
template<typename V> using meter_per_second_squared = div_quantity_t<meter_per_second<V>, second<V>>;
/** @} */

/** force */
template<typename V> using                  newton  = mul_quantity_t<kilogram<V>, meter_per_second_squared<V> >;

/** @{ energy */
template<typename V> using                   joule  = div_quantity_t< mul_quantity_t<kilogram<V>, square_meter<V>>
                                                                    , square_quantity_t<second<V>> >;
template<typename V> using             watt_second  = joule<V>;
template<typename V> using             watt_hour    = scale_by_t<hour_scaling, watt_second<V>>;
/** @} */

/** power */
template<typename V> using                    watt  = div_quantity_t<joule<V>,second<V>>;

/** @{ electric charge */
template<typename V> using                 coulomb  = mul_quantity_t<ampere<V>, second<V>>;
template<typename V> using           ampere_second  = coulomb<V>;
template<typename V> using           ampere_hour    = hour_scale<ampere_second<V>>;
/** @} */

/** voltage */
template<typename V> using                    volt  = div_quantity_t<joule<V>, coulomb<V>>;

/** resistance */
template<typename V> using                     ohm  = div_quantity_t<volt<V>, ampere<V>>;

/** @{ pressure */
template<typename V> using                 pascal_  = div_quantity_t<newton<V>, square_meter<V>>;
template<typename V> using                     bar  = scale_by_t<scale_t<100000>, pascal_<V>>;
/** @} */

/** volumetric flow rate */
template<typename V> using          liter_per_hour  = div_quantity_t<liter<V>, hour<V>>;
/** @} */

/**
 * @{
 *
 * @brief some quantities for electrical engineering
 *
 * This defines tagged quantities for active and reactive power and energy
 * used in electrical engineering.
 *
 * @see tag
 *
 * @tparam V  value type
 */
using reactive_power_tag = tag_t<struct reactive_power_tag_id>;
using apparent_power_tag = tag_t<struct apparent_power_tag_id>;
template<typename V> using       var                = tag_quantity_t<watt <V>,reactive_power_tag>;
template<typename V> using       var_second         = tag_quantity_t<joule<V>,reactive_power_tag>;
template<typename V> using       var_hour           = scale_by_t<hour_scaling,var_second<V>>;

template<typename V> using       voltampere         = tag_quantity_t<watt <V>,apparent_power_tag>;
template<typename V> using       voltampere_second  = tag_quantity_t<joule<V>,apparent_power_tag>;
template<typename V> using       voltampere_hour    = scale_by_t<hour_scaling, voltampere_second<V>>;
/** @} */

/**
 * @{
 *
 * @brief scalar quantities
 *
 * These define dimensionless quantities. The offer little advantage over a
 * plain value type when untagged, so they should always have a tag.
 *
 * However, untagged scalars are here defined as fraction (with their scaled
 * versions percent and permille), which allow to express the quotient of two
 * quantities of the same physical dimension.
 *
 * @tparam V  value type
 * @tparam T  tag
 * @tparam S  scale
 */
template< typename V, typename T, typename S=scale_t<1> >
                     using                   scalar = quantity<dimensionless, S, V, T>;

template<typename V> using                 fraction = scalar<V, no_tag, scale_t<1>>;
template<typename V> using                 percent  = scale_by_t<centi_scaling,fraction<V>>;
template<typename V> using                 permille = scale_by_t<milli_scaling,fraction<V>>;
/** @} */

/** @{
 * @brief dimensions for the above defined quantities of derived units
 *
 * These define the physical dimensions of the pre-defined derived units.
 * They can be used to declare templates which refer to a quantity of some
 * dimension at any scale.
 *
@code
template<typename Scale>
void f(unlib::quantity<unlib::velocity, Scale, double, void>);
@endcode
 *
 */
using frequency            = hertz                   <int>::dimension_type;
using area                 = square_meter            <int>::dimension_type;
using volume               = cubic_meter             <int>::dimension_type;
using velocity             = meter_per_second        <int>::dimension_type;
using acceleration         = meter_per_second_squared<int>::dimension_type;
using force                = newton                  <int>::dimension_type;
using energy               = joule                   <int>::dimension_type;
using power                = watt                    <int>::dimension_type;
using electric_charge      = coulomb                 <int>::dimension_type;
using voltage              = volt                    <int>::dimension_type;
using resistance           = ohm                     <int>::dimension_type;
using pressure             = pascal_                 <int>::dimension_type;
using volumetric_flow_rate = liter_per_hour          <int>::dimension_type;
/** @} */

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
/** defines a literal operator; do not invoke this macro directly */
#define UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,OpT_, R_)       inline constexpr auto operator""_##UnitShort_(OpT_ v) noexcept {return Sc_<Qty_<R_>>{static_cast<R_>(v)};}
/** defines a set of literal operators for a unit returning double and long long; do not invoke this macro directly */
#define UNLIB_DEFINE_SCALED_LITERAL(Qty_,Sc_,UnitShort_)                  namespace literals {                                                                      \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,long double       , floatpt_value_type) \
                                                                          UNLIB_DEFINE_LITERAL_OPERATOR(Qty_,Sc_,UnitShort_,unsigned long long, integer_value_type) \
                                                                          }
/** define the literal operators for a physical unit (without prefixes) */
#define UNLIB_DEFINE_LITERAL(Qty_,UnitShort_)                             UNLIB_DEFINE_SCALED_LITERAL(Qty_,no_scale,UnitShort_)
/** @{ micro unit prefixes */
/** define the literal operators for the micro unit prefixes femto-deci */
#define UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_FEMTO(Qty_,UnitShort_) UNLIB_DEFINE_SCALED_LITERAL(Qty_,deci , d##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,centi, c##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,milli, m##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,micro, u##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,nano , n##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,pico , p##UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,femto, f##UnitShort_)
/** define the literal operators for the micro unit prefixes atto-deci  */
#define UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO(Qty_,UnitShort_)  UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_FEMTO(Qty_,UnitShort_) \
                                                                          UNLIB_DEFINE_SCALED_LITERAL(Qty_,atto , a##UnitShort_)
/** @} */

/** @{ macro unit prefixes */
/** define the literal operators for the macro unit prefixes deca-kilo  */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_KILO(Qty_,UnitShort_)   UNLIB_DEFINE_SCALED_LITERAL(Qty_,deca ,dk##UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,hecto, h##UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,kilo , k##UnitShort_)
/** define the literal operators for the macro unit prefixes deca-mega  */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_MEGA(Qty_,UnitShort_)   UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_KILO(Qty_,UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,mega , M##UnitShort_)
/** define the literal operators for the macro unit prefixes deca-giga  */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_GIGA(Qty_,UnitShort_)   UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_MEGA(Qty_,UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,giga , G##UnitShort_)
/** define the literal operators for the macro unit prefixes deca-tera  */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA(Qty_,UnitShort_)   UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_GIGA(Qty_,UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,tera , T##UnitShort_)
/** define the literal operators for the macro unit prefixes deca-peta  */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_PETA(Qty_,UnitShort_)   UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA(Qty_,UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,peta , P##UnitShort_)
/** define the literal operators for the macro unit prefixes deca-exa   */
#define UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA(Qty_,UnitShort_)    UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_PETA(Qty_,UnitShort_) \
                                                                           UNLIB_DEFINE_SCALED_LITERAL(Qty_,exa  , E##UnitShort_)
/** @} */
/** @} */


/**
 * @{
 *
 * Literal operators for the basic and derived units known by this library
 */

/** @{ time */
UNLIB_DEFINE_LITERAL(second                  , s       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (second           , s   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (second           , s   )
UNLIB_DEFINE_LITERAL(minute                  , min     )
UNLIB_DEFINE_LITERAL(hour                    , h       )
UNLIB_DEFINE_LITERAL(day                     , d       )
UNLIB_DEFINE_LITERAL(week                    , week    )
/** @} */

/** @{ mass */
UNLIB_DEFINE_LITERAL(gram                    , g       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_FEMTO(gram             , g   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (gram             , g   )
UNLIB_DEFINE_LITERAL(ton                     , t       ) UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (ton              , t   )
/** @} */

/** length */
UNLIB_DEFINE_LITERAL(meter                   , m       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (meter            , m   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (meter            , m   )

/** current */
UNLIB_DEFINE_LITERAL(ampere                  , A       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (ampere           , A   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (ampere           , A   )

/** frequency */
UNLIB_DEFINE_LITERAL(hertz                   , Hz      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (hertz            , Hz  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (hertz            , Hz  )

/** @{ area */
UNLIB_DEFINE_LITERAL(square_millimeter       , mm2     )
UNLIB_DEFINE_LITERAL(square_centimeter       , cm2     )
UNLIB_DEFINE_LITERAL(square_centimeter       , dm2     )
UNLIB_DEFINE_LITERAL(square_meter            , m2      )
UNLIB_DEFINE_LITERAL(square_kilometer        , km2     )
UNLIB_DEFINE_LITERAL(hectare                 , ha      )
/** @} */

/** @{ volume */
UNLIB_DEFINE_LITERAL(cubic_millimeter        , mm3     )
UNLIB_DEFINE_LITERAL(cubic_centimeter        , cm3     )
UNLIB_DEFINE_LITERAL(cubic_decimeter         , dm3     )
UNLIB_DEFINE_LITERAL(cubic_meter             , m3      )
UNLIB_DEFINE_LITERAL(cubic_kilometer         , km3     )
UNLIB_DEFINE_LITERAL(milliliter              , ml      )
UNLIB_DEFINE_LITERAL(liter                   , l       )
/** @} */

/** @{ velocity */
UNLIB_DEFINE_LITERAL(meter_per_second        , m_per_s )
UNLIB_DEFINE_LITERAL(kilometer_per_hour      , km_per_h)
/** @} */

/** acceleration */
UNLIB_DEFINE_LITERAL(meter_per_second_squared, m_per_s2)

/** force */
UNLIB_DEFINE_LITERAL(newton                  , N       ) UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (newton           , N   )

/** substance amount */
UNLIB_DEFINE_LITERAL(mol                     , mol     )

/** luminous intensity */
UNLIB_DEFINE_LITERAL(candela                 , cd      )

/** @{ energy, work */
UNLIB_DEFINE_LITERAL(joule                   , J       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (joule            , J   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (joule            , J   )
UNLIB_DEFINE_LITERAL(watt_second             , Ws      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (watt_second      , Ws  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (watt_second      , Ws  )
UNLIB_DEFINE_LITERAL(watt_hour               , Wh      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (watt_hour        , Wh  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (watt_hour        , Wh  )
/** @} */

/** power */
UNLIB_DEFINE_LITERAL(watt                    , W       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (watt             , W   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (watt             , W   )

/** @{ electrical charge */
UNLIB_DEFINE_LITERAL(ampere_second           , As      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (ampere_second    , As  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_EXA  (ampere_second    , As  )
UNLIB_DEFINE_LITERAL(ampere_hour             , Ah      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (ampere_hour      , Ah  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (ampere_hour      , Ah  )
/** @} */

/** voltage */
UNLIB_DEFINE_LITERAL(volt                    , V       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (volt             , V   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (volt             , V   )

/** resistance */
UNLIB_DEFINE_LITERAL(ohm                     , O       ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (ohm              , O   )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (ohm              , O   )

/** @{ presure */
UNLIB_DEFINE_LITERAL(pascal_                 , Pa      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (pascal_          , Pa  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (pascal_          , Pa  )
UNLIB_DEFINE_LITERAL(bar                     , bar     ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (bar              , bar )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_GIGA (bar              , bar )
/** @} */

/** volumetric flow rate */
UNLIB_DEFINE_LITERAL(liter_per_hour          , l_per_h )

/** @{ AC reactive power and related units */
UNLIB_DEFINE_LITERAL(var                     , VAr     ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (var              , VAr )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (var              , VAr )
UNLIB_DEFINE_LITERAL(var_second              , VArs    ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (var_second       , VArs)
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (var_second       , VArs)
UNLIB_DEFINE_LITERAL(var_hour                , VArh    ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (var_hour         , VArh)
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (var_hour         , VArh)
/** @} */

/** @{ AC apparent power and related units */
UNLIB_DEFINE_LITERAL(voltampere              , VA      ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (voltampere       , VA  )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (voltampere       , VA  )
UNLIB_DEFINE_LITERAL(voltampere_second       , VAs     ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (voltampere_second, VAs )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (voltampere_second, VAs )
UNLIB_DEFINE_LITERAL(voltampere_hour         , VAh     ) UNLIB_DEFINE_METRIC_MICRO_PREFIXED_LITERAL_ATTO (voltampere_hour  , VAh )
                                                         UNLIB_DEFINE_METRIC_MACRO_PREFIXED_LITERAL_TERA (voltampere_hour  , VAh )
/** @} */

/** @} */

}

#endif /* UNLIB_COMMON_HPP */
