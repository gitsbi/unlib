# unlib

The unlib is a minimal, header-only, C++14-compatible [SI unit](https://en.wikipedia.org/wiki/International_System_of_Units) library, providing [quantities](#quantities) that behave like arithmetic types and feature [physical dimensions](#dimensions) (e.g. power), [scaling](#scales) (e.g., _kilo_), and [tagging](#tags) of units. If your code has to deal with physical units, you can use this library so that the compiler checks your usage of dimensions and your formulas at compile-time. 

The library tries to protect you from Murphy's doings and therefore errs on the side of requiring you to be explicit, rather than on the side of conveniences, where a design decision between these two had to be made.



# Getting Started

## Installations

The unlib is just a few of headers and the unit tests. If you are not interested in the latter, you can drop the headers into a folder named `unlib` that's in your path. However, you can also us it as a submodule in your git repository. It comes with a `CMakeLists.txt` file. 


## Using it in your code

Include `<unlib/common.hpp>` in your code in order to use the predefined [quantities](#quantities) and [literals](#literals). (`<unlib/common.hpp>` provides most of what you want when using this libraries. The two exceptions are `<unlib/limits.hpp>`, which provides a specialization of `std::numeric_limits<>` for quantities, and `<unlib/math.hpp>`, which provides a few mathematical functions and floating point comparison for quantities.)

If you want to make use of the predefined literal operators, you need to import the content of the `unlib::literals` namespace into yours.  

Then you can use the predefined quantities right away:

```cpp
using namespace unlib::literals;

unlib::kilo<unlib::watt_hour<double>> force = 1._W * 10._s;
```

In the above example, the right-hand side of the assignment operator calculates energy in `Ws` which is then used to initialize a quantity of the unit `kWh`. The conversion of the scale (kilo ==> unscaled) necessary for this is done implicitly during the assignment. 

The unit _watt_hour_ is of the physical dimension _energy_, _kilowatt_ is of _power_, and _second_ is of _time_. Knowing this, it is possible to write a generic function calculating the energy from power and time no matter what scaling: 

```cpp
template<typename V, typename PowerScale, typename TimeScale>
auto energy( const unlib::quantity<unlib::power, PowerScale, V>& power
           , const unlib::quantity<unlib::time , TimeScale , V>& time ) {
	return power * time;
}

auto nrg = energy(1._kW, 1._d);
```

Note that in this example, since the power is provided in kilowatt and the result type is **`auto`**, no automatic scaling will occur and the result will be a quantity of the unit kilo_watt_days. However, it can implicitly be rescaled any time:  

```cpp
unlib::kilo<unlib::watt_hour<double>> kWh = nrg;
```


# Known bugs

The library does not currently deal with the fact that the base unit of _mass_ is not _gram_, but _kilogrom_. As a result, units such as _Newton_, defined as `kg * m * s²`, will get they scale from the units they are calculated from, resulting in _Newton_ to have a _kilo_ scaling, rather than no scaling.

Since the work I currently do is not affected by this bug, I have not yet taken the time to redesign the library to fix this.


# Acknowledgments

This library is based on ideas and solutions which [melak47](https://github.com/melak47) came up with when, at a company we worked together at, he created such a beast for C++03 (which is infinitely harder, mind you). He, in turn, built on previous work done for [Boost.Unit](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_units.html). 


# Detailed documentation

At the moment, the only detailed documentation is the inline documentation in the library's headers. 



# Comparison to other work

There currently isn't a comprehensive comparisons with other libraries providing similar features, and I lack the time and, frankly, motivation, necessary to delve into those other libraries deep enough to make a comparison chart that's not inherently biased toward my library. However, before I started to write my own, I had looked at various other libraries, and there are a number of reasons why I did not use either of those, but decided to write my own. The main reasons are listed here: 

- [Mateusz Pusz' unit library](https://github.com/mpusz/units), which he tries to get into the next C++ standard, doesn't provide the C++14 compatibility needed for my work. 
- [Boost.Units](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_units.html) allows you to create your own unit system, whereas this library only enable you to use one system: SI. This of course greatly reduces the flexibility of unlib compared to Boost.Units, but makes it much easier to use, and very much faster to compile.
- [Nic Holthaus' unit library](https://github.com/nholthaus/units) has a rather unique way to scale units (think _kilo_, _nano_) which makes it impossible to, for example, write function templates that take any quantity, regardless of scale. It also doesn't provide any means for equivalent units that must be distinguished, like _active_, _reactive_, and _apparent_ power in EE. However, this both were must-have features for my work. 

This is, of course, _very biased_, because it just lists the criteria used to _dismiss_ the unit libraries we looked at in my job. That, however, is all I have at the moment. But if you, _or anyone else, FTM_, actually know any of those other libraries better than I do (which isn't hard, really), I'd be very interested to hear about more differences. 


# Main features


## Units

The SI unit system is built on the [seven base units](https://en.wikipedia.org/wiki/SI_base_unit):

1.  seconds  (_time_)
1.  kilogram (_mass_) (_Note:_ Due to [a known bug](#bugs), this is currently gram.)
1.  meter    (_length_)
1.  Ampere   (_current_)
1.  Kelvin   (_temperature_)
1.  mol      (_substance amount_)
1.  candela  (_luminous intensity_)

From these, all other units can be derived. 

(Note that this means that unlib does not provide units like, e.g., _kilobytes_. While in principle it is easy to add other units to this library's system of units, unlib was specifically not designed to be a framework for _any_ unit system. If you need to create your own type systems, you might want to look into [Boost.Units](https://www.boost.org/doc/libs/1_75_0/doc/html/boost_units.html). 

Note that the unlib, despite claiming to be a unit library, does not actually provide a single type for units. A [quantity](#quantities)'s unit is expressed through three template parameters of the quantity's type: [dimension](#dimensions), [scale](#scales), and [tag](#tags). 


## Dimensions

The base units, and all units which can be derived from them, are measuring units for physical dimensions. Dimensions can be described by the exponents of the underlying seven base units. A dimension, in the sense of this library, therefore is a (compile-time) set of the seven exponents, representing the seven base units. For example, electrical charge, measured in Coulomb, a.k.a. Ampere Second, can be defined this way:

```cpp
using my_dimension = unlib::dimension< /* time               */ unlib::ratio_t<1>  
                                     , /* mass               */ unlib::ratio_t<0> 
                                     , /* length             */ unlib::ratio_t<0> 
                                     , /* current            */ unlib::ratio_t<1> 
                                     , /* temperature        */ unlib::ratio_t<0> 
                                     , /* substance amount   */ unlib::ratio_t<0> 
                                     , /* luminous intensity */ unlib::ratio_t<0> >;
```

Since the library's way of telling these exponents from each other is their order, they must always be passed in the correct order, which is an invitation to silly errors. Therefore, dimensions can, and should, be built in different, less error-prone, ways. The `xxx_dimension_t` meta functions creates dimensions from the exponents of the seven base units, passed in any order. For example, using `mul_dimension_t`, electrical charge can be defined this way: 

```cpp
using electrical_charge = unlib::mul_dimension_t<unlib::current, unlib::time>;
```

This is both easier to write and read. If you want to measure the mass of cables, you could use a unit kg/m. This would be its dimension: 

```cpp
using mass_per_length = unlib::div_dimension_t< unlib::mass
                                              , unlib::length >
```

However, users usually won't have to deal directly with dimensions as they mostly use [quantities](#quantities). But if they do need to use dimensions, then the type of a quantity defines the nested type `dimension_type`, which could also be used to define this dimension:

```cpp
using namespace unlib::literals;
auto one_kg_per_meter = 1_kg/1_m;
using mass_per_length = decltype(one_kg_per_meter)::dimension_type;
```


## Scales

Units are scaled, where scales are rational numbers, represented by `std::ratio`. Due to the limitations of 64 bit integer arithmetic with `std::ratio`, of the [standard scales provided by the SI system](https://en.wikipedia.org/wiki/International_System_of_Units#Prefixes), this library only covers femto–exa. They are named like `unlib::femto_scaling`, `unlib::pico_scaling` etc., but they are simply aliases for `std::femto`, `std::pico`, etc. Besides those, noteworthy pre-defined scales are `no_scaling`, which denotes and unscaled quantity, as well as `minute_scaling`, `hour_scaling`, `day_scaling`, and `week_scaling`, which can be used to properly scale time quantities according to the Babylonian scaling system we use for time units.

The library also provides meta functions to create scaled quantities from the unscaled quantities: 

```cpp
using milligram = unlib::milli<unlib::gram>;
using  kilogram = unlib::kilo<unlib::gram>;
using       ton = unlib::kilo<kilogram>;       // humans don't use megagram
```


## Quantities

Quantities hold values in your code and mostly behave like C++' built-in arithmetic types: they can be added, multiplied, compared, etc. However, a quantity's template parameter specify the quantity's unit (_kilowatt_) and its value type (_**`double`**_), and quantities representing incompatible units or value types are incompatible to each other. The compiler keeps track of the correct types of the results of arithmetic operations. For example, multiplying a quantity of the unit _kW_ (_power_) with a quantity of the unit _h_ (_time_) will result in a quantity of the unit _kWh_ _(energy_), which cannot be assigned to a quantity of the unit _kW_.

An object of a quantity can be created from a value type explicitly. It cannot be implicitly created from a value type, though: 

```cpp
void f(unlib::quantity<unlib::mass,unlib::kilo,double>);

using kilogram = unlib::kilo<unlib::kilogram<double>>;

kilogram kg{42.}; // fine
f(kg);            // fine, too
f(kilogram{42.}); // also fine
f(42.);           // won't compile

using namespace unlib::literals; // needed for unit suffixes
f(42._kg);        // also works

gram g = kg;      // works, too, because scaling conversions are implicit
```

A quantity also cannot be used where its value type is needed. If you need to pass a quantity to an API that expects one of the built-in value types instead, you have to explicitly convert it:

```cpp
void f(double kg);

f(some_mass.get());                             // returns underlying value, whatever scaling
f(some_mass.get_scaled<unlib::kilo_scaling>()); // always returns kg 
f(get_scaled<unlib::no_scaling>(some_mass));    // always returns g 
```


## Values

Any integer or floating point type can be used as a quantity's value type. However, the literal operators provided by the library (in namespace `unlib::literals`) will return `unlib::literals::integer_value_type` (an alias for **`long`**) and `unlib::literals::floatpt_value_type` (an alias for **`double`**). If you need to use value types but also want to use literals, you have several choices:

You could wrap every constant into an explicit [cast](#conversions):
```cpp
auto distance{unlib::value_cast<long double>(42_m)};
```
Note that you do not have to explicitly name that cast's target's value type. This works, too: 
```cpp
unlib::kilometer<double> distance = unlib::value_cast(42_m);
```
Alternatively, you can change the literal operator result types through the macros `UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE` and `UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE`.


## Tags

Sometimes, different quantities which must not be confused are represented by the same physical unit. For example, in electrical engineering, when it comes to AC, there is active power, reactive power, and apparent power. All three are units of power and can be represented by the physical unit _Watt_. Nevertheless, usually they must not be confused. In order to allow this, quantities also have an optional template parameter `Tag`, which defaults to `no_tag`. A quantity with the tag `no_tag` is considered an untagged quantity. 

A tag consists of a tag ID and a tag ratio, which are passed to `unlib::tag_t`. Except for **`void`** (which will inevitable turn the tag into `unlib::no_tag`), any type, even an incomplete one, can be used for tag IDs. The only significance of these types is that they differ from each other. 

The ratio is a `std::ratio` and is used to keep track of tags when multiplying or dividing tagged types. (If the tag ratio is 0 (zero), then the tag's ID will become **`void`**, turning the tag into an `unlib::no_tag`.) Quantities with either differing tag IDs or differing tag ratios are considered to be of different type and cannot be assigned to each other. (There is, however, a [cast](#conversions) to circumvent this.)

Tags should be created by passing a tag ID to `unlib::tag_t`. The tag ratio should be omitted and defaults to 1/1. 

```cpp
using kiloWatt = unlib::quantity<unlib::power, unlib::kilo_scaling, unlib::integer_value_type>;
using kiloVar  = unlib::quantity<unlib::power, unlib::kilo_scaling, unlib::integer_value_type, unlib::tag_t<struct reactive_power_tag_id>>;

kiloWatt W = 42_W;
kiloWatt W2 = W;    // fine
kiloVar kvar = W;   // cannot assign due to different tag
```

(_Note:_ Using tags, the library already provides the three incompatible quantities `watt` for active power, `var` for reactive power, and `voltampere` for apparent power. Those are, again, templates, which can be parametrized with a value type.)

Two quantities can be added and subtracted if both have the same tag (which can be `no_tag`). They can be multiplied and divided if

 1. either operand has the `no_tag` tag or 
 1. both operands have the same tag.

If a quantity without a tag (that is, a quantity with the `no_tag`) is multiplied with a quantity with a tag (other than `no_tag`), the result will be tagged with the tag (other than `no_tag`). When multiplying and dividing tagged quantities, the library keeps track of the tag's "exponents" (how many times quantities of the same tag have been multiplied or divided with). 

Therefore you can, for example, multiply _reactive power_ with _time_, resulting in _reactive energy_. This divided by time results again in reactive power. If it is divided by reactive power, the result will be (untagged) time. You can also square _reactive power_, resulting in a quantity with a _reactive_ tag exponent of _2/1_. If you then divide this by _reactive power_, the result will be _reactive power_ again.)

Note that tags do not reflect all properties of their engineering counterparts. For example, dividing _reactive power_ by _voltage_, which is untagged, will have the resulting quantity of _current_ being tagged as _reactive_, which very likely won't make much sense. In these cases you will have to use a [cast](#conversions) to make the library submit to your application domain's rules. 

The following example from the domain of AC electrical engineering shows the calculation of apparent power from active and reactive power. Since reactive and apparent power are tagged quantities, and the rules of the application domain do cannot be implicitly expressed using the tagging mechanism, the tags have to be [casted](#conversions):

```cpp
template<typename AS, typename ReS, typename V>
auto apparent_power( const unlib::quantity<unlib::power, AS, V>& lhs
                   , const unlib::quantity<unlib::power, RS, V, unlib::reactive_power_tag>& rhs) {
	return unlib::tag_cast<unlib::apparent_power_tag>( unlib::sqrt( unlib::pow<2>(unlib::untag(lhs))
	                                                              + unlib::pow<2>(unlib::untag(rhs)) );
}

unlib::kilo<unlib::voltampere<double>> = apparent_power(10._kW, 5._kVA);
```


## Conversions

There are four different kind of casts available: 

 1. `value_cast` allows casting between units with different value types, e.g., seconds in `int` vs. seconds in `long long`.
 1. `scale_cast` allows casting between units with different scales, e.g., seconds and minutes. (_Note:_ Quantities that only differ in their scaling can implicitly be constructed from each other, so this cast will rarely ever be needed.)
 1. `tag_cast` allows casting between units with different tags, e.g., active and reactive power.
 1. `quantity_cast` allows casting between units where value types, scales, and tags might be different. 

All four types of casts come in two flavors. One needs the targeted value type, scale, tag, or quantity to be specified (just like `static_cast` etc. do): 

```cpp
unlib::scale_cast<unlib::milli>(any_weight);
```

The other flavor of the same cast does not need this. It returns a temporary object from which a quantity can be created or which can be assigned to a quantity. Depending on the quantity created from it, or it is assigned to, the requested conversion will be invoked automatically on assignment: 

```cpp
my_floating_hours float_hrs = unlib::value_cast(integer_seconds); // note: scale cast from secs to hrs is implicit
```

However, remember that this cast returns a temporary object which is not a quantity, and must be assigned to a quantity in order to be used. Specifically, it cannot be passed to functions expecting a quantity and therefore cannot be used in mathematical operations:

```cpp
// won't compile
unlib::kilo<unlib::watt> power = unlib::tag_cast(some_reactive_power_in_kW) 
                               / some_time;
```

In these cases, you need to explicitly mention the target you want to cast to, in order to immediately create an actual quantity: 

```cpp
unlib::kilo<unlib::watt> power = unlib::tag_cast<unlib::no_tag>(some_reactive_power_in_kW) 
                               / some_time;
```

For tag casts, there is also a function `untag()` which will remove any tag from a quantity:

```cpp
const auto kW = unlib::untag(1._kVar); // unlib::kilo<unlib::var<double>> ==> unlib::kilo<unlib::watt<double>>
```


## Literals

The library comes with predefined literal operators for the majority of the common quantities it predefines. You can find them in the header `<unlib/common.hpp>`. In order to be usable, those operators first must be brought into your current namespace through a _using directive_: 

```cpp
using namespace unlib::literals;

unlib::second<int> s = 1_h;

std::cout << s; // prints 3600
```

The resulting types of these operators are `unlib::litarals::integer_value_type` for integer litarals (`1_s`) and `unlib::literals::floatpt_value_type` for floating points literals (`1._kW`). They default to `long` and `double`, but can be changed by `#define`ing `UNLIB_LITERAL_OPERATOR_INTEGER_VALUE_TYPE` and  `UNLIB_LITERAL_OPERATOR_FLOATPT_VALUE_TYPE` during compilation. 


# Contributing

If you find bugs or want to request a feature, please [create an issue](https://github.com/gitsbi/unlib/issues). Of course, I'd be happy to look at a pull request as well. 

If you want to talk to me for whatever reason, I am [@tweetsbi on Twitter](https://twitter.com/tweetsbi).
