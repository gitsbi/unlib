# unlib
A minimal C++14 unit library

Unlib is a minimal, C++14-compatible [SI unit](https://en.wikipedia.org/wiki/International_System_of_Units) library, providing [quantities](#quantities) that mainly feature [units](#units) (e.g., _mass_) and [scales](#scales) (e.g., _kilo_). Quantities are what actually holds the typed values in your code. They mostly behave like C++' built-in arithmetic types: they can be added, multiplied, compared, etc.

Unlib tries to protect you from Murphy's doings and thus often errs on the side of requiring you to be explicit, rather than on the side of conveniences. However, since the library greatly improves type safety, some operations (e.g., typing) are more convenient than their C++ counterparts.


## Units

A Unit is an abstract concept representing a measure. This library provides the [seven base measures](https://en.wikipedia.org/wiki/SI_base_unit) of the SI system

1. time (_seconds_)
1. mass (_kilogram_)
1. length (_meter_)
1. current (_Ampere_)
1. luminosity (_candela_)
1. temperature (_Kelvin_)
1. substance amount (_mol_)

and any combination of those. (Note that this means that unlib does not provide units like, e.g., _kilobytes_.)

All basic units and many combined units are predefined in the library, but it is easy to define your own measurements. For example, while energy is already defined, you could just as well be define it yourself by multiplying and dividing the base units just like in the formula (kg⋅m<sup>2</sup>⋅s<sup>−2</sup>):

```c++
using energy = unlib::div_unit_t< unlib::mul_unit_t< unlib::mass
                                                   , unlib::square_unit_t<unlib::length> >
                                , unlib::square_unit_t<unlib::time> >
```

Energy can also be defined by providing all the exponents to the `unlib::unit_t` meta-function: 

```c++
using energy = unlib::unit_t< unlib::mass
                            , unlib::square_unit_t<unlib::length>
                            , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
```

However, energy is also power by time (P*t), and since the library already provides `unlib::power` and `unlib::time`, this could be used to simplify the formula considerable

```c++
using energy = unlib::mul_unit_t<unlib::power,unlib::time>;
```

Finally, the type of a quantity defines the nested type `unit_type`, which could be used:

```c++
using namespace unlib::literals;
auto power_quantity = (1_kg * 1_m * 1_m) / (1_s * 1_s);
using energy = decltype(power_quantity)::unit_type;
```


## Scales

Quantities are scaled, where scales are rational numbers, represented by `std::ratio`. Due to the limitations of 64 bit integer arithmetic with `std::ratio`, of the [standard scales provided by the SI system](https://en.wikipedia.org/wiki/International_System_of_Units#Prefixes) this library only covers atto–exa. They are named like `unlib::atto_scaling`, `unlib::femto_scaling` etc., but they are simply aliases for `std::atto`, `std::femto`, etc. 

Besides those, noteworthy pre-defined scales are `no_scaling`, as well as `minute_scaling`, `hour_scaling`, `day_scaling`, and `week_scaling`. The latter can be used to scale seconds.   

The library also provides meta functions to create scaled quantities from the basic quantities: 

```c++
using milligram = unlib::milli<unlib::gram>;
using  kilogram = unlib::kilo<unlib::gram>;
using       ton = unlib::kilo<kilogram>;       // humans don't use megagram
```


## Quantities

A quantity combines a [unit](#units) (_mass_), a [scale](#scales) (_kilo_), a specific [value type](#values) (_double_), and an optional [tag](#tags) (_reactive_): 

```c++
using kilogram = unlib::quantity<unlib::mass,unlib::kilo,double>. 
```

_(Note: All of unlib's predefined quantities, including _kilogram_, are templates that take the value type (here **`double`**) as a template parameter.)_ 

A quantity can be created from a value type explicitly: 

```c++
kilogram kg{42};
```

It cannot be implicitly created from a value type, though: 

```c++
void f(unlib::quantity<unlib::mass,unlib::kilo,double>);

f(kg);           // fine
f(42);           // won't compile
f(kilogram{42}); // also fine

using namespace unlib::literals; // needed for unit suffixes
f(42_kg);        // fine, too
```

Nor can a quantity be used where its value type is needed. If you need to pass a quantity to an API that expects one of the built-in value types instead, you have to explicitly convert it:

```c++
void f(double kg);

f(some_mass.get());                             // returns some_mass' underlying value, without regards for scaling
f(some_mass.get_scaled<unlib::kilo_scaling>()); // will return kg independent of some_mass' scale
```


## Values

Any integer or floating point type can be used as a quantity's value type. However, the literal operators provided by the library (in namespace `unlib::literals`) will return **`int`** and **`double`**. If you need to use `long long` or `long double`, but also want to use literals, you have three choices:

 1. Wrap every constant into an explicit [cast](#casts): `meter<long double> distance1{unlib::value_cast(42_m)};` (Note that you do not have to explicitly name that cast's target's value type. This works, too: `auto distance2{unlib::value_cast<long double>(42_m)};`.)

 1. Provide your own literal operators. (Note that this library provides several hundred literal operators.)

 1. Create a pull request making the types returned by the library's literal operators configurable. `:-)`


##Tags

In engineering, sometimes different quantities which must not be confused are represented by the same physical unit. For example, in electrical engineering, when it comes to AC, there is active power, reactive power, and apparent power. All three are units of power and can be represented by the physical unit _Watt_. Nevertheless, usually they must not be confused. In order to allow this, quantities also have an optional template parameter `Tag`, which defaults to `no_tag`. A quantity with the tag `no_tag` is considered an untagged quantity. 

The ratio is a `std::ratio` and is used to keep track of tags when multiplying or dividing tagged types. Quantities with either differing tag IDs or differing tag ratios are considered to be of different type and cannot be assigned to each other. (There is, however, a `tag_cast` to circumvent this.) 

A tag consists of a tag ID and a tag ratio, which are passed to `unlib::tag_t`. Except for **`void`** (which will turn the tag into an `unlib::no_tag`), any type, even an incomplete one, can be used for tag IDs. The only significance of these types is that they differ from each other. 

```c++
using kiloWatt = unlib::quantity<unlib::power, unlib::kilo_scaling, int>;
using kiloVar  = unlib::quantity<unlib::power, unlib::kilo_scaling, int, struct reactive_power>;

kiloWatt W = 42_W;
kiloWatt W2 = W;    // fine
kiloVar kvar = W;   // cannot assign due to different tag
```

(_Note:_ Using tags, the library already provides the three incompatible quantities `watt` for active power, `var` for reactive power, and `voltampere` for apparent power.)

Quantities can be multiplied and divided if

 1. either (or both) operand(s) have the `no_tag` tag or 
 1. both operands have the same tag.

When multiplying and dividing tagged quantities, the library keeps track of the tag's "exponents" (how many times quantities of the same tag have been multiplied or divided with). 

Therefore you can, for example, multiply _reactive power_ with _time_, resulting in _reactive energy_. This divided by reactive power results in untagged time. 

You can also multiply _reactive power_ with _reactive power_, resulting in a _reactive_ tag with a different exponent. If you then divide this by _reactive power_, the result will be _reactive power_ again.)

Note that tags do not reflect all properties of their engineering counterparts. For example, dividing _active power_ by _voltage_, which is untagged, will result in a current quantity tagged as _reactive_, which very likely won't make much sense. In these cases you will have to use `tag_cast` to make the library submit to your application domain's rules. 


## Conversions

There are four different kind of casts available: 

 1. `value_cast` allows casting between units with different value types, e.g., seconds in `int` vs. seconds in `long long`.
 1. `scale_cast` allows casting between units with different scales, e.g., seconds and minutes. (_Note:_ Quantities that only differ in their scaling can implicitly be constructed from each other, so this cast should rarely ever be needed.)
 1. `tag_cast` allows casting between units with different tags, e.g., active and reactive power.
 1. `quantity_cast` allows casting between units where value types, scales, and tags might be different. 

All four types of casts come in two flavors. One needs the targeted value type, scale, and/or tag to be specified (just like `static_cast` etc. do): 

```c++
  unlib::scale_cast<unlib::milli>(any_weight);
```

The other flavor of the same cast does not need this. It returns a temporary object from which a quantity can be created, and that can be assigned to a quantity. Depending on the quantity created from it, or it is assigned to, the respective conversion is invoked automatically: 

```c++
  my_floating_hours float_hrs = unlib::value_cast(integer_seconds); // note: scale cast from secs to hrs is implicit
```

Remember that this cast returns a temporary object which is not a quantity, and must be assigned to a quantity in order to be used. Specifically, it cannot be used in mathematical operations:

```c++
  // won't compile
  unlib::kilo<unlib::watt> power = unlib::tag_cast(some_reactive_power_in_kW) / some_time;
```

In these cases, you need to explicitly mention the target you want to cast to: 

```c++
  unlib::kilo<unlib::watt> power = unlib::tag_cast<unlib::no_tag>(some_reactive_power_in_kW) / some_time;
```
