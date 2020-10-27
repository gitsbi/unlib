# unlib
A minimal C++14 unit library

Unlib is the beginnings of a minimal, C++14-compatible [SI unit](https://en.wikipedia.org/wiki/International_System_of_Units) library, featuring [units](#units) (e.g., _mass_), [scales](#scales) (e.g., _kilo_), and [quantities](#quantities) (e.g., _kilogram_). Unlib tries to protect you from Murphy's doings and thus often errs on the side of requiring you to be explicit, rather than allowing implicit conversions or similar conveniences. 

## Units

A Unit is an abstract concept representing a measure. This library provides the [seven base measures](https://en.wikipedia.org/wiki/SI_base_unit) of the SI system

1. time (_seconds_)
1. mass (_kilogram_)
1. length (_meter_)
1. current (_Ampere_)
1. luminosity (_candela_)
1. temperature (_Kelvin_)
1. substance amount (_mol_)

and any combination of those. (Note that this means that unlib does not provide things like, e.g., _kilobytes_.)

All basic units and many combined units are predefined in the library, but it is easy to define your own measurements. For example, while energy is already defined, it could just as well be defined either by multiplying and dividing the base units just like in the formula (kg⋅m<sup>2</sup>⋅s<sup>−2</sup>):

```c++
using energy = unlib::div_unit_t< unlib::mul_unit_t< unlib::mass
                                                   , unlib::square_unit_t<unlib::length> >
                                , unlib::square_unit_t<unlib::time> >
```

Energy is also power by time (P*t), and since the library already provides `unlib::power`, this could be used to simplify the formula considerable

```c++
using energy = unlib::mul_unit_t<unlib::power,unlib::time>;
```

However, energy can also be defined by providing all the exponents to the `unlib::create_unit_t` meta-function: 

```c++
using energy = unlib::create_unit_t< unlib::mass
                                   , unlib::square_unit_t<unlib::length>
                                   , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
```

Finally, the type of a quantity defines the nested type `unit_type`, which could be used:

```c++
using namespace unlib::literals;
auto power_quantity = (1_kg * 1_m * 1_m) / (1_s * 1_s);
using energy = decltype(power_quantity)::unit_type;
```

## Quantities

A quantity combines a [unit](#units) (_mass_), a [scale](#scales) (_kilo_), and a specific value type (_double_): 

```c++
using kilogram = unlib::quantity<unlib::mass,unlib::kilo,double>`. 
```

(Note: Of course, _kilogram_ is a common enough quantity to be already defined in the library.) Quantities are what actually holds the typed values in your code. They mostly behave like C++' built-in arithmetic types: they can be added, multiplied, compared, etc. A quantity can be created from a value type explicitly: 

```c++
kilogram kg{42};
```

It cannot be implicitly created from a value type, though: 

```c++
void f(unlib::quantity<unlib::mass,unlib::kilo,double>);

f(kg);           // fine
f(42);           // won't compile
f(kilogram{42}); // fine
```

Nor can a quantity be used where its value type is needed. If you need to pass a quantity to an API that expects one of the built-in value types instead, you have to explicitly convert it:

```c++
void f(double kg);

f(kg.get());                             // returns kg
f(some_mass.get_scaled<unlib::kilo>()); // will return kg no matter what scale some_mass is
```

In engineering, sometimes different quantities that must not be confused are represented by the same physical unit. For example, in electrical engineering, when it comes to AC, there is active power, reactive power, and apparent power. All three are units of power and can be represented by the physical unit _Watt_. Nevertheless, in engineering they must not be confused. In order to allow this, quantities also have an optional template parameter `Tag` (which defaults to `void`). Quantities of different tags are considered of different type and cannot be assigned to each other.   
(The library provides the three incompatible quantities `watt` for active power, `var` for reactive power, and `voltampere` for apparent power.)

## Scales

Quantities are scaled, where scales are rational numbers. Due to the limitations of 64 bit interger arithmetic, of the [standard scales provided by SI](https://en.wikipedia.org/wiki/International_System_of_Units#Prefixes) this library only covers atto–exa. They are named like `unlib::atto_scaling`, `unlib::femto_scaling` etc. and are aliases for `std::atto`, `std::femto`, etc. The library also provides shorthands to create scaled quantities from the basic quantities: 

```c++
using milligram = unlib::milli<unlib::gram>;
using  kilogram = unlib::kilo<unlib::gram>;
using       ton = unlib::kilo<kilogram>;
```

Besides those, noteworthy scales supported are `no_scaling`, as well as `minute_scaling`, `hour_scaling`, `day_scaling`, and `week_scaling`, which scale seconds to their respective time unit.   

## Conversions

There are four different kind of casts available: 

 1. `value_cast` allows casting between units with different value types, e.g., seconds in `int` vs. seconds in `long long`.
 1. `scale_cast` allows casting between units with different scales, e.g., seconds and minutes. (_Note:_ Quantities that only differ in their scaling can implicitly be constructed from each other:)
 1. `tag_cast` allows casting between units with different tags, e.g., active and reactive power.
 1. `quantity_cast` allows casting between units where value types, scales, and tags might be different. 

All four types of casts come in two flavors. One needs the targetted value type, scale, and/or tag to be specified (just like `static_cast` etc. do): 

```c++
  unlib::scale_cast<unlib::milli>(any_weight);
```

The other does not need this. It returns a temporary object from which a quantity can be created, and that can be assigned to a quantity. Depending on the quantity created from it, or it is assigned to, the respective conversion is invoked automatically: 

```c++
  unlib::kilovoltampere reactive_power = unlib::tag_cast(some_active_power_in_kW);
```

Remember that this cast returns a temporary object which is not a quantity, and must be assigned to a quantity in order to be used.  
