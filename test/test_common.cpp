#include <unlib/common.hpp>
#include <string>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

TEST_CASE("common quantities") {
	SUBCASE("time") {
		using VT = long long;

		const unlib::second<VT>  s{ 1}; REQUIRE(s.get() ==  1);

		unlib::milli<unlib::second<VT>> ms{ s}; CHECK(ms.get() ==  s.get() * 1000);
		unlib::micro<unlib::second<VT>> us{ms}; CHECK(us.get() == ms.get() * 1000);
		unlib::nano <unlib::second<VT>> ns{us}; CHECK(ns.get() == us.get() * 1000);
		unlib::pico <unlib::second<VT>> ps{ns}; CHECK(ps.get() == ns.get() * 1000);
		unlib::femto<unlib::second<VT>> fs{ps}; CHECK(fs.get() == ps.get() * 1000);
		             unlib::second<VT>  s2{fs}; CHECK( s == s2);

		const VT seconds_per_week = 60*60*24*7;

		s2 = unlib::second<VT>{seconds_per_week}; REQUIRE(s2.get()  == seconds_per_week);
		unlib::minute<VT> min{ s2}; REQUIRE(min.get()); CHECK(min.get() == 60*24*7);
		unlib::  hour<VT>   h{min}; REQUIRE(  h.get()); CHECK(  h.get() ==    24*7);
		unlib::   day<VT>   d{  h}; REQUIRE(  d.get()); CHECK(  d.get() ==       7);
		unlib::  week<VT>   w{  d}; REQUIRE(  w.get()); CHECK(  w.get() ==       1);
		s2 = w; CHECK(s2.get() == seconds_per_week);

		using namespace unlib::literals;

		CHECK( typeid(1_fs ) == typeid(unlib::femto<unlib::second<VT>>) ); CHECK( 1_fs  == unlib::femto<unlib::second<VT>>{1} );
		CHECK( typeid(1_ps ) == typeid(unlib::pico <unlib::second<VT>>) ); CHECK( 1_ps  == unlib::pico <unlib::second<VT>>{1} );
		CHECK( typeid(1_ns ) == typeid(unlib::nano <unlib::second<VT>>) ); CHECK( 1_ns  == unlib::nano <unlib::second<VT>>{1} );
		CHECK( typeid(1_us ) == typeid(unlib::micro<unlib::second<VT>>) ); CHECK( 1_us  == unlib::micro<unlib::second<VT>>{1} );
		CHECK( typeid(1_ms ) == typeid(unlib::milli<unlib::second<VT>>) ); CHECK( 1_ms  == unlib::milli<unlib::second<VT>>{1} );
		CHECK( typeid(1_s  ) == typeid(             unlib::second<VT> ) ); CHECK( 1_s   ==              unlib::second<VT> {1} );
		CHECK( typeid(1_min) == typeid(             unlib::minute<VT> ) ); CHECK( 1_min ==              unlib::minute<VT> {1} );
		CHECK( typeid(1_h  ) == typeid(             unlib::  hour<VT> ) ); CHECK( 1_h   ==              unlib::  hour<VT> {1} );

		CHECK(1000_fs   ==  1_ps ); CHECK(is_near(1000._fs  , 1._ps ));
		CHECK(1000_ps   ==  1_ns ); CHECK(is_near(1000._ps  , 1._ns ));
		CHECK(1000_ns   ==  1_us ); CHECK(is_near(1000._ns  , 1._us ));
		CHECK(1000_us   ==  1_ms ); CHECK(is_near(1000._us  , 1._ms ));
		CHECK(1000_ms   ==  1_s  ); CHECK(is_near(1000._ms  , 1._s  ));
		CHECK(   1_min  == 60_s  ); CHECK(is_near(   1._min ,60._s  ));
		CHECK(   1_h    == 60_min); CHECK(is_near(   1._h   ,60._min));
		CHECK(   1_d    == 24_h  ); CHECK(is_near(   1._d   ,24._h  ));
		CHECK(   1_week ==  7_d  ); CHECK(is_near(   1._week, 7._d  ));
	}

	SUBCASE("mass") {
		using VT = long long;

		const unlib::kilo<unlib::gram<VT>> kg{ 1}; REQUIRE(kg.get() ==  1);

		                unlib::gram<VT>   g{kg}; REQUIRE(kg.get()); CHECK( g.get() == kg.get() * 1000);
		unlib::milli<unlib::gram<VT>> mg{ g}; REQUIRE( g.get()); CHECK(mg.get() ==  g.get() * 1000);
		unlib::micro<unlib::gram<VT>> ug{mg}; REQUIRE(mg.get()); CHECK(ug.get() == mg.get() * 1000);
		unlib::nano <unlib::gram<VT>> ng{ug}; REQUIRE(ug.get()); CHECK(ng.get() == ug.get() * 1000);
		unlib::pico <unlib::gram<VT>> pg{ng}; REQUIRE(ng.get()); CHECK(pg.get() == ng.get() * 1000);
		unlib::femto<unlib::gram<VT>> fg{pg}; REQUIRE(pg.get()); CHECK(fg.get() == pg.get() * 1000);
		unlib::kilo <unlib::gram<VT>> k2{fg}; REQUIRE(fg.get()); CHECK(kg == k2);

		k2 = kg * 1'000'000'000;
		            unlib::ton<VT>   t{k2}; REQUIRE( t.get()); CHECK( t.get() == (k2.get() /       1000));
		unlib::kilo<unlib::ton<VT>> kt{ t}; REQUIRE(kt.get()); CHECK(kt.get() == ( t.get() /       1000));
		unlib::mega<unlib::ton<VT>> Mt{kt}; REQUIRE(Mt.get()); CHECK(Mt.get() == (kt.get() /       1000));
		k2 = Mt;                            REQUIRE(k2.get()); CHECK(k2.get() == (kg.get() * 1000000000));

		using namespace unlib::literals;

		CHECK( typeid(1_fg) == typeid(unlib::femto<unlib::gram<VT>>) ); CHECK( 1_fg == unlib::femto<unlib::gram<VT>>{1} );
		CHECK( typeid(1_pg) == typeid(unlib::pico <unlib::gram<VT>>) ); CHECK( 1_pg == unlib::pico <unlib::gram<VT>>{1} );
		CHECK( typeid(1_ng) == typeid(unlib::nano <unlib::gram<VT>>) ); CHECK( 1_ng == unlib::nano <unlib::gram<VT>>{1} );
		CHECK( typeid(1_ug) == typeid(unlib::micro<unlib::gram<VT>>) ); CHECK( 1_ug == unlib::micro<unlib::gram<VT>>{1} );
		CHECK( typeid(1_mg) == typeid(unlib::milli<unlib::gram<VT>>) ); CHECK( 1_mg == unlib::milli<unlib::gram<VT>>{1} );
		CHECK( typeid(1_g ) == typeid(             unlib::gram<VT> ) ); CHECK( 1_g  ==              unlib::gram<VT> {1} );
		CHECK( typeid(1_kg) == typeid(unlib::kilo <unlib::gram<VT>>) ); CHECK( 1_kg == unlib::kilo <unlib::gram<VT>>{1} );
		CHECK( typeid(1_t ) == typeid(             unlib:: ton<VT> ) ); CHECK( 1_t  ==              unlib:: ton<VT> {1} );
		CHECK( typeid(1_Mt) == typeid(unlib::mega <unlib:: ton<VT>>) ); CHECK( 1_Mt == unlib::mega <unlib:: ton<VT>>{1} );

		CHECK(1000_fg == 1_pg ); CHECK(is_near(1000._fg,1._pg) );
		CHECK(1000_pg == 1_ng ); CHECK(is_near(1000._pg,1._ng) );
		CHECK(1000_ng == 1_ug ); CHECK(is_near(1000._ng,1._ug) );
		CHECK(1000_ug == 1_mg ); CHECK(is_near(1000._ug,1._mg) );
		CHECK(1000_mg == 1_g  ); CHECK(is_near(1000._mg,1._g ) );
		CHECK(1000_g  == 1_kg ); CHECK(is_near(1000._g ,1._kg) );
		CHECK(1000_kg == 1_t  ); CHECK(is_near(1000._kg,1._t ) );
		CHECK(1000_t  == 1_kt ); CHECK(is_near(1000._t ,1._kt) );
		CHECK(1000_kt == 1_Mt ); CHECK(is_near(1000._kt,1._Mt) );
	}

	SUBCASE("length") {
		using VT = long long;

		const unlib::kilo<unlib::meter<VT>> km{ 1}; REQUIRE(km.get() ==  1);

		unlib::femto<unlib::meter<VT>> fm{km}; REQUIRE(km.get()); CHECK(fm.get() == km.get() * 1000'000'000'000'000'000);
		unlib::pico <unlib::meter<VT>> pm{fm}; REQUIRE(fm.get()); CHECK(pm.get() == fm.get() / 1000);
		unlib::nano <unlib::meter<VT>> nm{pm}; REQUIRE(pm.get()); CHECK(nm.get() == pm.get() / 1000);
		unlib::micro<unlib::meter<VT>> um{nm}; REQUIRE(nm.get()); CHECK(um.get() == nm.get() / 1000);
		unlib::milli<unlib::meter<VT>> mm{um}; REQUIRE(um.get()); CHECK(mm.get() == um.get() / 1000);
		unlib::centi<unlib::meter<VT>> cm{mm}; REQUIRE(mm.get()); CHECK(cm.get() == mm.get() /   10);
		unlib::deci <unlib::meter<VT>> dm{cm}; REQUIRE(cm.get()); CHECK(dm.get() == cm.get() /   10);
		             unlib::meter<VT>   m{dm}; REQUIRE(dm.get()); CHECK( m.get() == dm.get() /   10);

		unlib::kilo <unlib::meter<VT>> k2{m};  CHECK( k2 == km );

		using namespace unlib::literals;

		CHECK( typeid(1_fm) == typeid(unlib::femto<unlib::meter<VT>>) ); CHECK( 1_fm == unlib::femto<unlib::meter<VT>>{1} );
		CHECK( typeid(1_pm) == typeid(unlib::pico <unlib::meter<VT>>) ); CHECK( 1_pm == unlib::pico <unlib::meter<VT>>{1} );
		CHECK( typeid(1_nm) == typeid(unlib::nano <unlib::meter<VT>>) ); CHECK( 1_nm == unlib::nano <unlib::meter<VT>>{1} );
		CHECK( typeid(1_um) == typeid(unlib::micro<unlib::meter<VT>>) ); CHECK( 1_um == unlib::micro<unlib::meter<VT>>{1} );
		CHECK( typeid(1_mm) == typeid(unlib::milli<unlib::meter<VT>>) ); CHECK( 1_mm == unlib::milli<unlib::meter<VT>>{1} );
		CHECK( typeid(1_m ) == typeid(             unlib::meter<VT> ) ); CHECK( 1_m  ==              unlib::meter<VT> {1} );
		CHECK( typeid(1_km) == typeid(unlib::kilo <unlib::meter<VT>>) ); CHECK( 1_km == unlib::kilo <unlib::meter<VT>>{1} );

		CHECK(1000_fm == 1_pm ); CHECK(is_near(1000._fm,1._pm) );
		CHECK(1000_pm == 1_nm ); CHECK(is_near(1000._pm,1._nm) );
		CHECK(1000_nm == 1_um ); CHECK(is_near(1000._nm,1._um) );
		CHECK(1000_um == 1_mm ); CHECK(is_near(1000._um,1._mm) );
		CHECK(1000_mm == 1_m  ); CHECK(is_near(1000._mm,1._m ) );
		CHECK( 100_cm == 1_m  ); CHECK(is_near( 100._cm,1._m ) );
		CHECK(  10_dm == 1_m  ); CHECK(is_near(  10._dm,1._m ) );
		CHECK(1000_m  == 1_km ); CHECK(is_near(1000._m ,1._km) );
	}

	SUBCASE("area") {
		using VT = long long;

		CHECK(test::is_same_unit<unlib::area, unlib::square_meter     <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::area, unlib::square_kilometer <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::area, unlib::             are <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::area, unlib::         hectare <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::area, unlib::square_centimeter<VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::area, unlib::square_millimeter<VT>::unit_type>());

		using namespace unlib::literals;
		CHECK(10000_cm2 ==       1_m2         ); CHECK(is_near(10000._cm2,      1._m2            ));
		CHECK(    1_ha  ==   10000_m2         ); CHECK(is_near(    1._ha ,  10000._m2            ));
		CHECK(    1_cm2 ==     100_mm2        ); CHECK(is_near(    1._cm2,    100._mm2           ));
		CHECK(    1_m2  == 1000000_mm2        ); CHECK(is_near(    1._m2 ,1000000._mm2           ));
		CHECK(    1_ha  == unlib::are<VT>{100}); CHECK(is_near(    1._ha ,unlib::are<double>{100}));
	}

	SUBCASE("volume") {
		using VT = long long;

		CHECK(test::is_same_unit<unlib::volume, unlib::cubic_millimeter<VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::volume, unlib::cubic_centimeter<VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::volume, unlib::cubic_meter     <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::volume, unlib::cubic_kilometer <VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::volume, unlib::           liter<VT>::unit_type>());
		CHECK(test::is_same_unit<unlib::volume, unlib::      milliliter<VT>::unit_type>());

		using namespace unlib::literals;
		CHECK(1_km3 == 1000000000_m3 ); CHECK(is_near(1._km3, 1000000000._m3 ));
		CHECK(1_m3  ==       1000_l  ); CHECK(is_near(1._m3 ,       1000._l  ));
		CHECK(1_l   ==       1000_ml ); CHECK(is_near(1._l  ,       1000._ml ));
		CHECK(1_l   ==       1000_cm3); CHECK(is_near(1._l  ,       1000._cm3));
		CHECK(1_ml  ==       1000_mm3); CHECK(is_near(1._ml ,       1000._mm3));
	}

	SUBCASE("temperature") {
		using VT = long long;

		const unlib::degree_kelvin    <VT> d_k{ 1}; REQUIRE(d_k.get() ==  1);
		const unlib::degree_celsius   <VT> d_c{ 1}; REQUIRE(d_c.get() ==  1);
		const unlib::degree_fahrenheit<VT> d_f{ 1}; REQUIRE(d_f.get() ==  1);

		using namespace unlib::literals;

		CHECK( unlib::scale_cast<unlib::degree_fahrenheit<VT>::scale_type>(unlib::degree_kelvin <VT>(10)) == unlib::degree_fahrenheit<VT>(18) );
		CHECK( unlib::scale_cast<unlib::degree_fahrenheit<VT>::scale_type>(unlib::degree_celsius<VT>(10)) == unlib::degree_fahrenheit<VT>(18) );

	}

	SUBCASE("current") {
		using VT = long long;

		const unlib::ampere<VT> A{ 1}; REQUIRE(A.get() ==  1);

		unlib::femto<unlib::ampere<VT>> fA{ A}; REQUIRE( A.get()); CHECK(fA.get() ==  A.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ampere<VT>> pA{fA}; REQUIRE(fA.get()); CHECK(pA.get() == fA.get() / 1000);
		unlib::nano <unlib::ampere<VT>> nA{pA}; REQUIRE(pA.get()); CHECK(nA.get() == pA.get() / 1000);
		unlib::micro<unlib::ampere<VT>> uA{nA}; REQUIRE(nA.get()); CHECK(uA.get() == nA.get() / 1000);
		unlib::milli<unlib::ampere<VT>> mA{uA}; REQUIRE(uA.get()); CHECK(mA.get() == uA.get() / 1000);
		             unlib::ampere<VT>  A2{mA}; REQUIRE(mA.get()); CHECK(A2.get() ==  A.get());

		A2 *= 1000'000'000ll;
		unlib::kilo <unlib::ampere<VT>> kA{A2}; REQUIRE(A2.get()); CHECK(kA.get() == A2.get() / 1000);
		unlib::mega <unlib::ampere<VT>> MA{kA}; REQUIRE(kA.get()); CHECK(MA.get() == kA.get() / 1000);
		unlib::giga <unlib::ampere<VT>> GA{MA}; REQUIRE(MA.get()); CHECK(GA.get() == MA.get() / 1000);

		A2 = GA;  CHECK( A2 == unlib::ampere<VT>{mA}*1000'000'000 );

		using namespace unlib::literals;

		CHECK( typeid(1_fA) == typeid(unlib::femto<unlib::ampere<VT>>) ); CHECK( 1_fA == (unlib::femto<unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_pA) == typeid(unlib::pico <unlib::ampere<VT>>) ); CHECK( 1_pA == (unlib::pico <unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_nA) == typeid(unlib::nano <unlib::ampere<VT>>) ); CHECK( 1_nA == (unlib::nano <unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_uA) == typeid(unlib::micro<unlib::ampere<VT>>) ); CHECK( 1_uA == (unlib::micro<unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_mA) == typeid(unlib::milli<unlib::ampere<VT>>) ); CHECK( 1_mA == (unlib::milli<unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_A ) == typeid(             unlib::ampere<VT> ) ); CHECK( 1_A  == (             unlib::ampere<VT> {1} ) );
		CHECK( typeid(1_kA) == typeid(unlib::kilo <unlib::ampere<VT>>) ); CHECK( 1_kA == (unlib::kilo <unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_MA) == typeid(unlib::mega <unlib::ampere<VT>>) ); CHECK( 1_MA == (unlib::mega <unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_GA) == typeid(unlib::giga <unlib::ampere<VT>>) ); CHECK( 1_GA == (unlib::giga <unlib::ampere<VT>>{1} ) );
		CHECK( typeid(1_TA) == typeid(unlib::tera <unlib::ampere<VT>>) ); CHECK( 1_TA == (unlib::tera <unlib::ampere<VT>>{1} ) );

		CHECK(1000_fA == 1_pA ); CHECK(is_near(1000._fA,1._pA) );
		CHECK(1000_pA == 1_nA ); CHECK(is_near(1000._pA,1._nA) );
		CHECK(1000_nA == 1_uA ); CHECK(is_near(1000._nA,1._uA) );
		CHECK(1000_uA == 1_mA ); CHECK(is_near(1000._uA,1._mA) );
		CHECK(1000_mA == 1_A  ); CHECK(is_near(1000._mA,1._A ) );
		CHECK(1000_A  == 1_kA ); CHECK(is_near(1000._A ,1._kA) );
		CHECK(1000_kA == 1_MA ); CHECK(is_near(1000._kA,1._MA) );
		CHECK(1000_MA == 1_GA ); CHECK(is_near(1000._MA,1._GA) );
		CHECK(1000_GA == 1_TA ); CHECK(is_near(1000._GA,1._TA) );
	}

	SUBCASE("frequency") {
		using VT = long long;

		// Wikipedia: F = T^-1
		using frequency = unlib::create_unit_t< unlib::reciprocal_unit_t<unlib::time> >;
		CHECK( test::is_same_unit<frequency,unlib::hertz<VT>::unit_type>() );

		const unlib::hertz<VT> Hz{ 1}; REQUIRE(Hz.get() ==  1);

		unlib::femto<unlib::hertz<VT>> fHz{ Hz}; REQUIRE( Hz.get()); CHECK(fHz.get() ==  Hz.get() * 1000'000'000'000'000);
		unlib::pico <unlib::hertz<VT>> pHz{fHz}; REQUIRE(fHz.get()); CHECK(pHz.get() == fHz.get() / 1000);
		unlib::nano <unlib::hertz<VT>> nHz{pHz}; REQUIRE(pHz.get()); CHECK(nHz.get() == pHz.get() / 1000);
		unlib::micro<unlib::hertz<VT>> uHz{nHz}; REQUIRE(nHz.get()); CHECK(uHz.get() == nHz.get() / 1000);
		unlib::milli<unlib::hertz<VT>> mHz{uHz}; REQUIRE(uHz.get()); CHECK(mHz.get() == uHz.get() / 1000);
		             unlib::hertz<VT>   H2{mHz}; REQUIRE(mHz.get()); CHECK( H2.get() ==  Hz.get());

		H2 *= 1000'000'000ll;
		unlib::kilo <unlib::hertz<VT>> kHz{ H2}; REQUIRE( H2.get()); CHECK(kHz.get() ==  H2.get() / 1000);
		unlib::mega <unlib::hertz<VT>> MHz{kHz}; REQUIRE(kHz.get()); CHECK(MHz.get() == kHz.get() / 1000);
		unlib::giga <unlib::hertz<VT>> GHz{MHz}; REQUIRE(MHz.get()); CHECK(GHz.get() == MHz.get() / 1000);

		H2 = GHz;  CHECK( H2 == unlib::hertz<VT>{mHz}*1000'000'000 );

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::hertz<double>>(1. / 1._s) );

		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_fHz) ); CHECK(1_fHz == unlib::femto<unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_pHz) ); CHECK(1_pHz == unlib::pico <unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_nHz) ); CHECK(1_nHz == unlib::nano <unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_uHz) ); CHECK(1_uHz == unlib::micro<unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_mHz) ); CHECK(1_mHz == unlib::milli<unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_Hz) ); CHECK(1_Hz == unlib::hertz<VT> {1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_kHz) ); CHECK(1_kHz == unlib::kilo <unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_MHz) ); CHECK(1_MHz == unlib::mega <unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_GHz) ); CHECK(1_GHz == unlib::giga <unlib::hertz<VT>>{1} );
		CHECK(test::is_same_unit<unlib::hertz<VT>>(1_THz) ); CHECK(1_THz == unlib::tera <unlib::hertz<VT>>{1} );

		CHECK(1000_fHz == 1_pHz ); CHECK(is_near(1000._fHz,1._pHz) );
		CHECK(1000_pHz == 1_nHz ); CHECK(is_near(1000._pHz,1._nHz) );
		CHECK(1000_nHz == 1_uHz ); CHECK(is_near(1000._nHz,1._uHz) );
		CHECK(1000_uHz == 1_mHz ); CHECK(is_near(1000._uHz,1._mHz) );
		CHECK(1000_mHz == 1_Hz  ); CHECK(is_near(1000._mHz,1._Hz ) );
		CHECK(1000_Hz  == 1_kHz ); CHECK(is_near(1000._Hz ,1._kHz) );
		CHECK(1000_kHz == 1_MHz ); CHECK(is_near(1000._kHz,1._MHz) );
		CHECK(1000_MHz == 1_GHz ); CHECK(is_near(1000._MHz,1._GHz) );
		CHECK(1000_GHz == 1_THz ); CHECK(is_near(1000._GHz,1._THz) );
	}

	SUBCASE("voltage") {
		using VT = long long;

		// Wikipedia: U = M?L^2?I^-1?T^-3
		using voltage = unlib::create_unit_t< unlib::mass
		                                    , unlib::square_unit_t<unlib::length>
		                                    , unlib::reciprocal_unit_t<unlib::current>
		                                    , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CHECK(test::is_same_unit<voltage,unlib::volt<VT>::unit_type>());

		const unlib::volt<VT> V{ 1}; REQUIRE(V.get() ==  1);

		unlib::femto<unlib::volt<VT>> fV{ V}; REQUIRE( V.get()); CHECK(fV.get() ==  V.get() * 1000'000'000'000'000);
		unlib::pico <unlib::volt<VT>> pV{fV}; REQUIRE(fV.get()); CHECK(pV.get() == fV.get() / 1000);
		unlib::nano <unlib::volt<VT>> nV{pV}; REQUIRE(pV.get()); CHECK(nV.get() == pV.get() / 1000);
		unlib::micro<unlib::volt<VT>> uV{nV}; REQUIRE(nV.get()); CHECK(uV.get() == nV.get() / 1000);
		unlib::milli<unlib::volt<VT>> mV{uV}; REQUIRE(uV.get()); CHECK(mV.get() == uV.get() / 1000);
		             unlib::volt<VT>  V2{mV}; REQUIRE(mV.get()); CHECK(V2.get() ==  V.get());

		V2 *= 1000'000'000ll;
		unlib::kilo <unlib::volt<VT>> kV{V2}; REQUIRE(V2.get()); CHECK(kV.get() == V2.get() / 1000);
		unlib::mega <unlib::volt<VT>> MV{kV}; REQUIRE(kV.get()); CHECK(MV.get() == kV.get() / 1000);
		unlib::giga <unlib::volt<VT>> GV{MV}; REQUIRE(MV.get()); CHECK(GV.get() == MV.get() / 1000);

		V2 = GV;  CHECK( V2 == unlib::volt<VT>{mV}*1000'000'000 );

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::volt<double>>((1._g * 1._m * 1._m) / (1._A * 1._s * 1._s * 1._s)) );

		CHECK(test::is_same_unit<unlib::volt<VT>>(1_fV) ); CHECK(1_fV == unlib::femto<unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_pV) ); CHECK(1_pV == unlib::pico <unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_nV) ); CHECK(1_nV == unlib::nano <unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_uV) ); CHECK(1_uV == unlib::micro<unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_mV) ); CHECK(1_mV == unlib::milli<unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_V) ); CHECK(1_V == unlib::volt<VT> {1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_kV) ); CHECK(1_kV == unlib::kilo <unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_MV) ); CHECK(1_MV == unlib::mega <unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_GV) ); CHECK(1_GV == unlib::giga <unlib::volt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::volt<VT>>(1_TV) ); CHECK(1_TV == unlib::tera <unlib::volt<VT>>{1} );

		CHECK(1000_fV == 1_pV ); CHECK(is_near(1000._fV,1._pV) );
		CHECK(1000_pV == 1_nV ); CHECK(is_near(1000._pV,1._nV) );
		CHECK(1000_nV == 1_uV ); CHECK(is_near(1000._nV,1._uV) );
		CHECK(1000_uV == 1_mV ); CHECK(is_near(1000._uV,1._mV) );
		CHECK(1000_mV == 1_V  ); CHECK(is_near(1000._mV,1._V ) );
		CHECK(1000_V  == 1_kV ); CHECK(is_near(1000._V ,1._kV) );
		CHECK(1000_kV == 1_MV ); CHECK(is_near(1000._kV,1._MV) );
		CHECK(1000_MV == 1_GV ); CHECK(is_near(1000._MV,1._GV) );
		CHECK(1000_GV == 1_TV ); CHECK(is_near(1000._GV,1._TV) );
	}

	SUBCASE("resistance") {
		using VT = long long;

		// Wikipedia: R = M?L^2?I^-2?T^-3
		using resistance = unlib::create_unit_t< unlib::mass
		                                       , unlib::square_unit_t<unlib::length>
		                                       , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::current>>
		                                       , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CHECK(test::is_same_unit<resistance, unlib::ohm<VT>::unit_type>() );

		const unlib::ohm<VT> O{ 1}; REQUIRE(O.get() ==  1);

		unlib::femto<unlib::ohm<VT>> fO{ O}; REQUIRE( O.get()); CHECK(fO.get() ==  O.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ohm<VT>> pO{fO}; REQUIRE(fO.get()); CHECK(pO.get() == fO.get() / 1000);
		unlib::nano <unlib::ohm<VT>> nO{pO}; REQUIRE(pO.get()); CHECK(nO.get() == pO.get() / 1000);
		unlib::micro<unlib::ohm<VT>> uO{nO}; REQUIRE(nO.get()); CHECK(uO.get() == nO.get() / 1000);
		unlib::milli<unlib::ohm<VT>> mO{uO}; REQUIRE(uO.get()); CHECK(mO.get() == uO.get() / 1000);
		             unlib::ohm<VT>  O2{mO}; REQUIRE(mO.get()); CHECK(O2.get() ==  O.get());

		O2 *= 1000'000'000ll;
		unlib::kilo <unlib::ohm<VT>> kO{O2}; REQUIRE(O2.get()); CHECK(kO.get() == O2.get() / 1000);
		unlib::mega <unlib::ohm<VT>> MO{kO}; REQUIRE(kO.get()); CHECK(MO.get() == kO.get() / 1000);
		unlib::giga <unlib::ohm<VT>> GO{MO}; REQUIRE(MO.get()); CHECK(GO.get() == MO.get() / 1000);

		O2 = GO;  CHECK(O2 == unlib::ohm<VT>{mO}*1000'000'000 );

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::ohm<double>>(1._V / 1._A) );

		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_fO) ); CHECK(1_fO == unlib::femto<unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_pO) ); CHECK(1_pO == unlib::pico <unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_nO) ); CHECK(1_nO == unlib::nano <unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_uO) ); CHECK(1_uO == unlib::micro<unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_mO) ); CHECK(1_mO == unlib::milli<unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_O) ); CHECK(1_O == unlib::ohm<VT> {1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_kO) ); CHECK(1_kO == unlib::kilo <unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_MO) ); CHECK(1_MO == unlib::mega <unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_GO) ); CHECK(1_GO == unlib::giga <unlib::ohm<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ohm<VT>>(1_TO) ); CHECK(1_TO == unlib::tera <unlib::ohm<VT>>{1} );

		CHECK(1000_fO == 1_pO ); CHECK(is_near(1000._fO,1._pO) );
		CHECK(1000_pO == 1_nO ); CHECK(is_near(1000._pO,1._nO) );
		CHECK(1000_nO == 1_uO ); CHECK(is_near(1000._nO,1._uO) );
		CHECK(1000_uO == 1_mO ); CHECK(is_near(1000._uO,1._mO) );
		CHECK(1000_mO == 1_O  ); CHECK(is_near(1000._mO,1._O ) );
		CHECK(1000_O  == 1_kO ); CHECK(is_near(1000._O ,1._kO) );
		CHECK(1000_kO == 1_MO ); CHECK(is_near(1000._kO,1._MO) );
		CHECK(1000_MO == 1_GO ); CHECK(is_near(1000._MO,1._GO) );
		CHECK(1000_GO == 1_TO ); CHECK(is_near(1000._GO,1._TO) );
	}

	SUBCASE("power") {
		using VT = long long;

		using power = unlib::create_unit_t< unlib::mass
		                                  , unlib::square_unit_t<unlib::length>
		                                  , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CHECK(test::is_same_unit<power, unlib::watt<VT>::unit_type>() );

		const unlib::      watt<VT>    W{1}; REQUIRE(  W.get() == 1);
		const unlib::       var<VT> va_r{1}; REQUIRE(va_r.get() == 1);
		const unlib::voltampere<VT>   va{1}; REQUIRE( va.get() == 1);

		unlib::femto<unlib::watt<VT>> fW{ W}; REQUIRE( W.get()); CHECK(fW.get() ==  W.get() * 1000'000'000'000'000);
		unlib::pico <unlib::watt<VT>> pW{fW}; REQUIRE(fW.get()); CHECK(pW.get() == fW.get() / 1000);
		unlib::nano <unlib::watt<VT>> nW{pW}; REQUIRE(pW.get()); CHECK(nW.get() == pW.get() / 1000);
		unlib::micro<unlib::watt<VT>> uW{nW}; REQUIRE(nW.get()); CHECK(uW.get() == nW.get() / 1000);
		unlib::milli<unlib::watt<VT>> mW{uW}; REQUIRE(uW.get()); CHECK(mW.get() == uW.get() / 1000);

		unlib::      watt<VT>   W2{mW};                  REQUIRE(  W2.get()); CHECK(  W2.get() == W.get());
		unlib::       var<VT> var2{unlib::tag_cast(W2)}; REQUIRE(var2.get()); CHECK(var2.get() == W.get());
		unlib::voltampere<VT>  va2{unlib::tag_cast(W2)}; REQUIRE( va2.get()); CHECK( va2.get() == W.get());

		/*  W2 = var2; var2 =   W2; */   W2 = unlib::tag_cast(var2); var2 = unlib::tag_cast(  W2);
		/*  W2 =  va2;  va2 =   W2; */   W2 = unlib::tag_cast( va2);  va2 = unlib::tag_cast(  W2);
		/*var2 =  va2;  va2 = var2; */ var2 = unlib::tag_cast( va2);  va2 = unlib::tag_cast(var2);


		W2 *= 1000'000'000ll;
		unlib::kilo <unlib::watt<VT>> kW{W2}; REQUIRE(W2.get()); CHECK(kW.get() == W2.get() / 1000);
		unlib::mega <unlib::watt<VT>> MW{kW}; REQUIRE(kW.get()); CHECK(MW.get() == kW.get() / 1000);
		unlib::giga <unlib::watt<VT>> GW{MW}; REQUIRE(MW.get()); CHECK(GW.get() == MW.get() / 1000);

		 W2 = GW;  CHECK(W2 == unlib::watt<VT>{mW}*1000'000'000 );

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::watt<double>>(1._V * 1._A) );

		CHECK(test::is_same_unit<unlib::watt<VT>>(1_fW) ); CHECK(1_fW == unlib::femto<unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_pW) ); CHECK(1_pW == unlib::pico <unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_nW) ); CHECK(1_nW == unlib::nano <unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_uW) ); CHECK(1_uW == unlib::micro<unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_mW) ); CHECK(1_mW == unlib::milli<unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_W) ); CHECK(1_W == unlib::watt<VT> {1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_kW) ); CHECK(1_kW == unlib::kilo <unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_MW) ); CHECK(1_MW == unlib::mega <unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_GW) ); CHECK(1_GW == unlib::giga <unlib::watt<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt<VT>>(1_TW) ); CHECK(1_TW == unlib::tera <unlib::watt<VT>>{1} );

		CHECK(test::is_same_unit<unlib::var<VT>>(1_fVAr) ); CHECK(1_fVAr == unlib::femto<unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_pVAr) ); CHECK(1_pVAr == unlib::pico <unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_nVAr) ); CHECK(1_nVAr == unlib::nano <unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_uVAr) ); CHECK(1_uVAr == unlib::micro<unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_mVAr) ); CHECK(1_mVAr == unlib::milli<unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_VAr) ); CHECK(1_VAr == unlib::var<VT> {1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_kVAr) ); CHECK(1_kVAr == unlib::kilo <unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_MVAr) ); CHECK(1_MVAr == unlib::mega <unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_GVAr) ); CHECK(1_GVAr == unlib::giga <unlib::var<VT>>{1} );
		CHECK(test::is_same_unit<unlib::var<VT>>(1_TVAr) ); CHECK(1_TVAr == unlib::tera <unlib::var<VT>>{1} );

		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_fVA) ); CHECK(1_fVA == unlib::femto<unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_pVA) ); CHECK(1_pVA == unlib::pico <unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_nVA) ); CHECK(1_nVA == unlib::nano <unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_uVA) ); CHECK(1_uVA == unlib::micro<unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_mVA) ); CHECK(1_mVA == unlib::milli<unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_VA) ); CHECK(1_VA == unlib::voltampere<VT> {1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_kVA) ); CHECK(1_kVA == unlib::kilo <unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_MVA) ); CHECK(1_MVA == unlib::mega <unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_GVA) ); CHECK(1_GVA == unlib::giga <unlib::voltampere<VT>>{1} );
		CHECK(test::is_same_unit<unlib::voltampere<VT>>(1_TVA) ); CHECK(1_TVA == unlib::tera <unlib::voltampere<VT>>{1} );

		CHECK(1000_fW == 1_pW ); CHECK(is_near(1000._fW,1._pW) );
		CHECK(1000_pW == 1_nW ); CHECK(is_near(1000._pW,1._nW) );
		CHECK(1000_nW == 1_uW ); CHECK(is_near(1000._nW,1._uW) );
		CHECK(1000_uW == 1_mW ); CHECK(is_near(1000._uW,1._mW) );
		CHECK(1000_mW == 1_W  ); CHECK(is_near(1000._mW,1._W ) );
		CHECK(1000_W  == 1_kW ); CHECK(is_near(1000._W ,1._kW) );
		CHECK(1000_kW == 1_MW ); CHECK(is_near(1000._kW,1._MW) );
		CHECK(1000_MW == 1_GW ); CHECK(is_near(1000._MW,1._GW) );
		CHECK(1000_GW == 1_TW ); CHECK(is_near(1000._GW,1._TW) );

		CHECK(1000_VAr  == 1_kVAr ); CHECK(is_near(1000._VAr ,1._kVAr) );
		CHECK(1000_kVAr == 1_MVAr ); CHECK(is_near(1000._kVAr,1._MVAr) );
		CHECK(1000_MVAr == 1_GVAr ); CHECK(is_near(1000._MVAr,1._GVAr) );
		CHECK(1000_GVAr == 1_TVAr ); CHECK(is_near(1000._GVAr,1._TVAr) );

		CHECK(1000_VA  == 1_kVA ); CHECK(is_near(1000._VA ,1._kVA) );
		CHECK(1000_kVA == 1_MVA ); CHECK(is_near(1000._kVA,1._MVA) );
		CHECK(1000_MVA == 1_GVA ); CHECK(is_near(1000._MVA,1._GVA) );
		CHECK(1000_GVA == 1_TVA ); CHECK(is_near(1000._GVA,1._TVA) );
	}

	SUBCASE("energy") {
		using VT = long long;

		// Wikipedia: E = L^2?M?T^-2
		using energy = unlib::create_unit_t< unlib::square_unit_t<unlib::length>
		                                   , unlib::mass
		                                   , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
		CHECK(test::is_same_unit<energy, unlib::energy>() );
		CHECK(test::is_same_unit<energy, unlib::watt_hour<VT>::unit_type>() );

		const unlib::watt_hour  <VT> Wh{1 };           REQUIRE(Wh.get() == 1);
		const unlib::watt_second<VT> Ws{Wh};           REQUIRE(Ws.get() == Wh.get()*3600);
		const unlib::      joule<VT>  J{tag_cast(Ws)}; REQUIRE( J.get() == Ws.get());

		unlib::femto<unlib::watt_second<VT>> fWs{ Ws};           REQUIRE( Ws.get()); CHECK(fWs.get() ==  Ws.get() * 1000'000'000'000'000);
		unlib::pico <unlib::watt_second<VT>> pWs{fWs};           REQUIRE(fWs.get()); CHECK(pWs.get() == fWs.get() / 1000);
		unlib::nano <unlib::watt_second<VT>> nWs{pWs};           REQUIRE(pWs.get()); CHECK(nWs.get() == pWs.get() / 1000);
		unlib::micro<unlib::watt_second<VT>> uWs{nWs};           REQUIRE(nWs.get()); CHECK(uWs.get() == nWs.get() / 1000);
		unlib::milli<unlib::watt_second<VT>> mWs{uWs};           REQUIRE(uWs.get()); CHECK(mWs.get() == uWs.get() / 1000);
		             unlib::watt_second<VT>  Ws2{mWs};           REQUIRE(mWs.get()); CHECK(Ws2.get() ==  Ws.get());
		             unlib::watt_hour  <VT>  Wh2{Ws2};           REQUIRE(Ws2.get()); CHECK(Ws2.get() ==  Ws.get());
		             unlib::      joule<VT>   J2{tag_cast(Ws2)}; REQUIRE( J2.get()); CHECK( J2.get() == Ws.get());

		Wh2 *= 1000000000ll;
		unlib::kilo <unlib::watt_hour  <VT>> kWh{Wh2}; REQUIRE(Wh2.get()); CHECK(kWh.get() == Wh2.get() / 1000);
		unlib::mega <unlib::watt_hour  <VT>> MWh{kWh}; REQUIRE(kWh.get()); CHECK(MWh.get() == kWh.get() / 1000);
		unlib::giga <unlib::watt_hour  <VT>> GWh{MWh}; REQUIRE(MWh.get()); CHECK(GWh.get() == MWh.get() / 1000);

		Wh2 = GWh;
		CHECK(Wh2 == Wh*1000000000ll);

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::watt_hour<double>>(1._W * 1._h));

		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_fWs) ); CHECK(1_fWs == unlib::femto<unlib::watt_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_pWs) ); CHECK(1_pWs == unlib::pico <unlib::watt_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_nWs) ); CHECK(1_nWs == unlib::nano <unlib::watt_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_uWs) ); CHECK(1_uWs == unlib::micro<unlib::watt_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_mWs) ); CHECK(1_mWs == unlib::milli<unlib::watt_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_second<VT>>(1_Ws) ); CHECK(1_Ws == unlib::watt_second<VT> {1} );

		CHECK(test::is_same_unit<unlib::joule<VT>>(1_J) ); CHECK(1_J == unlib::joule      <VT> {1} );

		CHECK(test::is_same_unit<unlib::watt_hour<VT>>(1_Wh) ); CHECK(1_Wh == unlib::watt_hour  <VT> {1} );
		CHECK(test::is_same_unit<unlib::watt_hour<VT>>(1_kWh) ); CHECK(1_kWh == unlib::kilo<unlib::watt_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_hour<VT>>(1_MWh) ); CHECK(1_MWh == unlib::mega<unlib::watt_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_hour<VT>>(1_GWh) ); CHECK(1_GWh == unlib::giga<unlib::watt_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::watt_hour<VT>>(1_TWh) ); CHECK(1_TWh == unlib::tera<unlib::watt_hour  <VT>>{1} );

		CHECK(1_Ws == unlib::watt_second<VT>{unlib::tag_cast(1_J)}); CHECK(is_near(1._Ws,unlib::watt_second<double>{unlib::tag_cast(1._J)}));

		CHECK(1000_fWs == 1_pWs ); CHECK(is_near(1000._fWs,1._pWs) );
		CHECK(1000_pWs == 1_nWs ); CHECK(is_near(1000._pWs,1._nWs) );
		CHECK(1000_nWs == 1_uWs ); CHECK(is_near(1000._nWs,1._uWs) );
		CHECK(1000_uWs == 1_mWs ); CHECK(is_near(1000._uWs,1._mWs) );
		CHECK(1000_mWs == 1_Ws  ); CHECK(is_near(1000._mWs,1._Ws ) );
		CHECK(3600_Ws  == 1_Wh  ); CHECK(is_near(3600._Ws ,1._Wh ) );
		CHECK(1000_Wh  == 1_kWh ); CHECK(is_near(1000._Wh ,1._kWh) );
		CHECK(1000_kWh == 1_MWh ); CHECK(is_near(1000._kWh,1._MWh) );
		CHECK(1000_MWh == 1_GWh ); CHECK(is_near(1000._MWh,1._GWh) );
		CHECK(1000_GWh == 1_TWh ); CHECK(is_near(1000._GWh,1._TWh) );
	}

	SUBCASE("electric charge") {
		using VT = long long;

		// Wikipedia: Q = I?T
		using charge = unlib::create_unit_t< unlib::current
		                                   , unlib::time >;
		CHECK(test::is_same_unit<charge, unlib::ampere_hour<VT>::unit_type>() );

		const unlib::ampere_hour  <VT> Ah{1 }; REQUIRE(Ah.get() == 1);
		const unlib::ampere_second<VT> As{Ah}; REQUIRE(As.get() == Ah.get()*3600);

		unlib::femto<unlib::ampere_second<VT>> fAs{ As}; REQUIRE( As.get()); CHECK(fAs.get() ==  As.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ampere_second<VT>> pAs{fAs}; REQUIRE(fAs.get()); CHECK(pAs.get() == fAs.get() / 1000);
		unlib::nano <unlib::ampere_second<VT>> nAs{pAs}; REQUIRE(pAs.get()); CHECK(nAs.get() == pAs.get() / 1000);
		unlib::micro<unlib::ampere_second<VT>> uAs{nAs}; REQUIRE(nAs.get()); CHECK(uAs.get() == nAs.get() / 1000);
		unlib::milli<unlib::ampere_second<VT>> mAs{uAs}; REQUIRE(uAs.get()); CHECK(mAs.get() == uAs.get() / 1000);
		             unlib::ampere_second<VT>  As2{mAs}; REQUIRE(mAs.get()); CHECK(As2.get() ==  As.get());

		             unlib::ampere_hour  <VT>  Ah2{As2}; REQUIRE(As2.get()); CHECK(As2.get() ==  As.get());
		Ah2 *= 1000000000ll;
		unlib::kilo <unlib::ampere_hour  <VT>> kAh{Ah2}; REQUIRE(Ah2.get()); CHECK(kAh.get() == Ah2.get() / 1000);
		unlib::mega <unlib::ampere_hour  <VT>> MAh{kAh}; REQUIRE(kAh.get()); CHECK(MAh.get() == kAh.get() / 1000);
		unlib::giga <unlib::ampere_hour  <VT>> GAh{MAh}; REQUIRE(MAh.get()); CHECK(GAh.get() == MAh.get() / 1000);

		Ah2 = GAh;
		CHECK(Ah2 == Ah*1000000000ll);

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::ampere_hour<double>>(1._A * 1._h) );

		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_fAs) ); CHECK(1_fAs == unlib::femto<unlib::ampere_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_pAs) ); CHECK(1_pAs == unlib::pico <unlib::ampere_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_nAs) ); CHECK(1_nAs == unlib::nano <unlib::ampere_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_uAs) ); CHECK(1_uAs == unlib::micro<unlib::ampere_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_mAs) ); CHECK(1_mAs == unlib::milli<unlib::ampere_second<VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_second<VT>::unit_type>(1_As) ); CHECK(1_As == unlib::ampere_second<VT> {1} );

		CHECK(test::is_same_unit<unlib::ampere_hour<VT>::unit_type>(1_Ah) ); CHECK(1_Ah == unlib::ampere_hour  <VT> {1} );
		CHECK(test::is_same_unit<unlib::ampere_hour<VT>::unit_type>(1_kAh) ); CHECK(1_kAh == unlib::kilo <unlib::ampere_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_hour<VT>::unit_type>(1_MAh) ); CHECK(1_MAh == unlib::mega <unlib::ampere_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_hour<VT>::unit_type>(1_GAh) ); CHECK(1_GAh == unlib::giga <unlib::ampere_hour  <VT>>{1} );
		CHECK(test::is_same_unit<unlib::ampere_hour<VT>::unit_type>(1_TAh) ); CHECK(1_TAh == unlib::tera <unlib::ampere_hour  <VT>>{1} );

		CHECK(1000_fAs == 1_pAs ); CHECK(is_near(1000._fAs,1._pAs) );
		CHECK(1000_pAs == 1_nAs ); CHECK(is_near(1000._pAs,1._nAs) );
		CHECK(1000_nAs == 1_uAs ); CHECK(is_near(1000._nAs,1._uAs) );
		CHECK(1000_uAs == 1_mAs ); CHECK(is_near(1000._uAs,1._mAs) );
		CHECK(1000_mAs == 1_As  ); CHECK(is_near(1000._mAs,1._As ) );
		CHECK(3600_As  == 1_Ah  ); CHECK(is_near(3600._As ,1._Ah ) );
		CHECK(1000_Ah  == 1_kAh ); CHECK(is_near(1000._Ah ,1._kAh) );
		CHECK(1000_kAh == 1_MAh ); CHECK(is_near(1000._kAh,1._MAh) );
		CHECK(1000_MAh == 1_GAh ); CHECK(is_near(1000._MAh,1._GAh) );
		CHECK(1000_GAh == 1_TAh ); CHECK(is_near(1000._GAh,1._TAh) );
	}

	SUBCASE("pressure") {
		using VT = long long;

		// Wikipedia: P =  	M?L^-1?T^-2
		using pressure = unlib::create_unit_t< unlib::mass
		                                     , unlib::reciprocal_unit_t<unlib::length>
		                                     , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
		CHECK(test::is_same_unit<pressure, unlib::pressure>() );
		CHECK(test::is_same_unit<pressure, unlib::bar<VT>::unit_type>() );

		const unlib::    bar<VT> bar1{1   }; REQUIRE(bar1.get() == 1);
		const unlib::pascal_<VT>   Pa{bar1}; REQUIRE(  Pa.get() == bar1.get()*100000);

		unlib::femto<unlib::bar    <VT>> fbar{bar1}; REQUIRE(bar1.get()); CHECK(fbar.get() == bar1.get() * 1000'000'000'000'000);
		unlib::pico <unlib::bar    <VT>> pbar{fbar}; REQUIRE(fbar.get()); CHECK(pbar.get() == fbar.get() / 1000);
		unlib::nano <unlib::bar    <VT>> nbar{pbar}; REQUIRE(pbar.get()); CHECK(nbar.get() == pbar.get() / 1000);
		unlib::micro<unlib::bar    <VT>> ubar{nbar}; REQUIRE(nbar.get()); CHECK(ubar.get() == nbar.get() / 1000);
		unlib::milli<unlib::bar    <VT>> mbar{ubar}; REQUIRE(ubar.get()); CHECK(mbar.get() == ubar.get() / 1000);
		             unlib::bar    <VT>  bar2{mbar}; REQUIRE(mbar.get()); CHECK(bar2.get() == bar1.get());

		             unlib::pascal_<VT>   Pa2{bar2}; REQUIRE(bar2.get()); CHECK(bar2.get() == bar1.get());

		Pa2 *= 1000000000ll;
		unlib::kilo <unlib::pascal_<VT>> kPa{Pa2}; REQUIRE(Pa2.get()); CHECK(kPa.get() == Pa2.get() / 1000);
		unlib::hecto<unlib::pascal_<VT>> hPa{Pa2}; REQUIRE(Pa2.get()); CHECK(hPa.get() == Pa2.get() / 100 );
		unlib::mega <unlib::pascal_<VT>> MPa{kPa}; REQUIRE(kPa.get()); CHECK(MPa.get() == kPa.get() / 1000);
		unlib::giga <unlib::pascal_<VT>> GPa{MPa}; REQUIRE(MPa.get()); CHECK(GPa.get() == MPa.get() / 1000);

		Pa2 = GPa;
		CHECK(Pa2 == Pa*1000000000ll);

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::pascal_<double>::unit_type>(1._g / (1._m * 1._s * 1._s)) );

		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_pbar) ); CHECK(1_pbar == unlib::pico <unlib::bar<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_nbar) ); CHECK(1_nbar == unlib::nano <unlib::bar<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_ubar) ); CHECK(1_ubar == unlib::micro<unlib::bar<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_mbar) ); CHECK(1_mbar == unlib::milli<unlib::bar<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_bar ) ); CHECK(1_bar  == unlib::bar<VT> {1} );

		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_Pa ) ); CHECK(1_Pa  == unlib::pascal_<VT> {1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_hPa) ); CHECK(1_hPa == unlib::hecto<unlib::pascal_<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_kPa) ); CHECK(1_kPa == unlib::kilo <unlib::pascal_<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_MPa) ); CHECK(1_MPa == unlib::mega <unlib::pascal_<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_GPa) ); CHECK(1_GPa == unlib::giga <unlib::pascal_<VT>>{1} );
		CHECK(test::is_same_unit<unlib::pascal_<VT>::unit_type>(1_TPa) ); CHECK(1_TPa == unlib::tera <unlib::pascal_<VT>>{1} );

		CHECK(1000_pbar == 1_nbar ); CHECK(is_near(1000._pbar,1._nbar) );
		CHECK(1000_nbar == 1_ubar ); CHECK(is_near(1000._nbar,1._ubar) );
		CHECK(1000_ubar == 1_mbar ); CHECK(is_near(1000._ubar,1._mbar) );
		CHECK(1000_mbar == 1_bar  ); CHECK(is_near(1000._mbar,1._bar ) );
		CHECK( 100_kPa  == 1_bar  ); CHECK(is_near( 100._kPa ,1._bar ) );
		CHECK( 100_Pa   == 1_hPa  ); CHECK(is_near( 100._Pa  ,1._hPa ) );
		CHECK(1000_Pa   == 1_kPa  ); CHECK(is_near(1000._Pa  ,1._kPa ) );
		CHECK(1000_kPa  == 1_MPa  ); CHECK(is_near(1000._kPa ,1._MPa ) );
		CHECK(1000_MPa  == 1_GPa  ); CHECK(is_near(1000._MPa ,1._GPa ) );
		CHECK(1000_GPa  == 1_TPa  ); CHECK(is_near(1000._GPa ,1._TPa ) );
	}

	SUBCASE("velocity") {
		using VT = long long;

		// Wikipedia: a =  	L?T^-1
		using velocity = unlib::create_unit_t< unlib::length
		                                     , unlib::reciprocal_unit_t<unlib::time> >;
		CHECK(test::is_same_unit<unlib::velocity, velocity>() );
		CHECK(test::is_same_unit<unlib::velocity, unlib::meter_per_second<VT>::unit_type>() );
		CHECK(test::is_same_unit<unlib::velocity, unlib::kilometer_per_hour<VT>::unit_type>() );

		const unlib::    meter_per_second<VT> mps {100}; REQUIRE( mps.get() == 100            );
		const unlib::kilometer_per_hour  <VT> kmph{mps}; CHECK  (kmph.get() == mps.get()*36/10);

		using namespace unlib::literals;

		CHECK(test::is_same_unit<unlib::meter_per_second<double>>(1._m / 1._s) );
		CHECK(test::is_same_unit<unlib::kilometer_per_hour<double>>(1._km / 1._h) );

		CHECK(test::is_same_unit<unlib::    meter_per_second<VT>>(1_m_per_s ) ); CHECK(1_m_per_s  == unlib::    meter_per_second<VT>{1} );
		CHECK(test::is_same_unit<unlib::kilometer_per_hour<VT  >>(1_km_per_h) ); CHECK(1_km_per_h == unlib::kilometer_per_hour  <VT>{1} );

		CHECK(10_m_per_s == 36_km_per_h); CHECK(is_near(1.0_m_per_s,3.6_km_per_h));
	}

	SUBCASE("volumetric flow rate") {
		using VT = long long;

		CHECK(test::is_same_unit<unlib::liter_per_hour<VT>::unit_type, unlib::volumetric_flow_rate>);

		using namespace unlib::literals;
		CHECK(test::is_same_unit<unlib::liter_per_hour<double>>(1._l / 1._h) );
		CHECK(test::is_same_unit<unlib::liter_per_hour<VT    >>(1_l_per_h));

		CHECK(1_l_per_h == unlib::liter_per_hour<VT>{1}); CHECK(is_near(1._l_per_h,unlib::liter_per_hour<double>{1}));
	}
}
