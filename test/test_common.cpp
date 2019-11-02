#include <unlib/common.hpp>
#include <string>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch.hpp>

#include <unlib/test/unlib_test.hpp>

CATCH_TEST_CASE("common quantities") {
	CATCH_SECTION("time") {
		using VT = long long;

		const unlib::second<VT>  s{ 1}; CATCH_REQUIRE(s.get() ==  1);

		unlib::milli<unlib::second<VT>> ms{ s}; CATCH_CHECK(ms.get() ==  s.get() * 1000);
		unlib::micro<unlib::second<VT>> us{ms}; CATCH_CHECK(us.get() == ms.get() * 1000);
		unlib::nano <unlib::second<VT>> ns{us}; CATCH_CHECK(ns.get() == us.get() * 1000);
		unlib::pico <unlib::second<VT>> ps{ns}; CATCH_CHECK(ps.get() == ns.get() * 1000);
		unlib::femto<unlib::second<VT>> fs{ps}; CATCH_CHECK(fs.get() == ps.get() * 1000);
		             unlib::second<VT>  s2{fs}; CATCH_CHECK( s == s2);

		const VT seconds_per_week = 60*60*24*7;

		s2 = unlib::second<VT>{seconds_per_week}; CATCH_REQUIRE(s2.get()  == seconds_per_week);
		unlib::minute<VT> min{ s2}; CATCH_REQUIRE(min.get()); CATCH_CHECK(min.get() == 60*24*7);
		unlib::  hour<VT>   h{min}; CATCH_REQUIRE(  h.get()); CATCH_CHECK(  h.get() ==    24*7);
		unlib::   day<VT>   d{  h}; CATCH_REQUIRE(  d.get()); CATCH_CHECK(  d.get() ==       7);
		unlib::  week<VT>   w{  d}; CATCH_REQUIRE(  w.get()); CATCH_CHECK(  w.get() ==       1);
		s2 = w; CATCH_CHECK(s2.get() == seconds_per_week);

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1_fs ) == typeid(unlib::femto<unlib::second<VT>>) ); CATCH_CHECK( 1_fs  == unlib::femto<unlib::second<VT>>{1} );
		CATCH_CHECK( typeid(1_ps ) == typeid(unlib::pico <unlib::second<VT>>) ); CATCH_CHECK( 1_ps  == unlib::pico <unlib::second<VT>>{1} );
		CATCH_CHECK( typeid(1_ns ) == typeid(unlib::nano <unlib::second<VT>>) ); CATCH_CHECK( 1_ns  == unlib::nano <unlib::second<VT>>{1} );
		CATCH_CHECK( typeid(1_us ) == typeid(unlib::micro<unlib::second<VT>>) ); CATCH_CHECK( 1_us  == unlib::micro<unlib::second<VT>>{1} );
		CATCH_CHECK( typeid(1_ms ) == typeid(unlib::milli<unlib::second<VT>>) ); CATCH_CHECK( 1_ms  == unlib::milli<unlib::second<VT>>{1} );
		CATCH_CHECK( typeid(1_s  ) == typeid(             unlib::second<VT> ) ); CATCH_CHECK( 1_s   ==              unlib::second<VT> {1} );
		CATCH_CHECK( typeid(1_min) == typeid(             unlib::minute<VT> ) ); CATCH_CHECK( 1_min ==              unlib::minute<VT> {1} );
		CATCH_CHECK( typeid(1_h  ) == typeid(             unlib::  hour<VT> ) ); CATCH_CHECK( 1_h   ==              unlib::  hour<VT> {1} );

		CATCH_CHECK(1000_fs   ==  1_ps ); CATCH_CHECK(1000._fs   ==  1._ps );
		CATCH_CHECK(1000_ps   ==  1_ns ); CATCH_CHECK(1000._ps   ==  1._ns );
		CATCH_CHECK(1000_ns   ==  1_us ); CATCH_CHECK(1000._ns   ==  1._us );
		CATCH_CHECK(1000_us   ==  1_ms ); CATCH_CHECK(1000._us   ==  1._ms );
		CATCH_CHECK(1000_ms   ==  1_s  ); CATCH_CHECK(1000._ms   ==  1._s  );
		CATCH_CHECK(   1_min  == 60_s  ); CATCH_CHECK(   1._min  == 60._s  );
		CATCH_CHECK(   1_h    == 60_min); CATCH_CHECK(   1._h    == 60._min);
		CATCH_CHECK(   1_d    == 24_h  ); CATCH_CHECK(   1._d    == 24._h  );
		CATCH_CHECK(   1_week ==  7_d  ); CATCH_CHECK(   1._week ==  7._d  );
	}

	CATCH_SECTION("mass") {
		using VT = long long;

		const unlib::kilo<unlib::gram<VT>> kg{ 1}; CATCH_REQUIRE(kg.get() ==  1);

		                unlib::gram<VT>   g{kg}; CATCH_REQUIRE(kg.get()); CATCH_CHECK( g.get() == kg.get() * 1000);
		unlib::milli<unlib::gram<VT>> mg{ g}; CATCH_REQUIRE( g.get()); CATCH_CHECK(mg.get() ==  g.get() * 1000);
		unlib::micro<unlib::gram<VT>> ug{mg}; CATCH_REQUIRE(mg.get()); CATCH_CHECK(ug.get() == mg.get() * 1000);
		unlib::nano <unlib::gram<VT>> ng{ug}; CATCH_REQUIRE(ug.get()); CATCH_CHECK(ng.get() == ug.get() * 1000);
		unlib::pico <unlib::gram<VT>> pg{ng}; CATCH_REQUIRE(ng.get()); CATCH_CHECK(pg.get() == ng.get() * 1000);
		unlib::femto<unlib::gram<VT>> fg{pg}; CATCH_REQUIRE(pg.get()); CATCH_CHECK(fg.get() == pg.get() * 1000);
		unlib::kilo <unlib::gram<VT>> k2{fg}; CATCH_REQUIRE(fg.get()); CATCH_CHECK(kg == k2);

		k2 = kg * 1'000'000'000;
		            unlib::ton<VT>   t{k2}; CATCH_REQUIRE( t.get()); CATCH_CHECK( t.get() == (k2.get() /       1000));
		unlib::kilo<unlib::ton<VT>> kt{ t}; CATCH_REQUIRE(kt.get()); CATCH_CHECK(kt.get() == ( t.get() /       1000));
		unlib::mega<unlib::ton<VT>> Mt{kt}; CATCH_REQUIRE(Mt.get()); CATCH_CHECK(Mt.get() == (kt.get() /       1000));
		k2 = Mt;                            CATCH_REQUIRE(k2.get()); CATCH_CHECK(k2.get() == (kg.get() * 1000000000));

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1_fg) == typeid(unlib::femto<unlib::gram<VT>>) ); CATCH_CHECK( 1_fg == unlib::femto<unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_pg) == typeid(unlib::pico <unlib::gram<VT>>) ); CATCH_CHECK( 1_pg == unlib::pico <unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_ng) == typeid(unlib::nano <unlib::gram<VT>>) ); CATCH_CHECK( 1_ng == unlib::nano <unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_ug) == typeid(unlib::micro<unlib::gram<VT>>) ); CATCH_CHECK( 1_ug == unlib::micro<unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_mg) == typeid(unlib::milli<unlib::gram<VT>>) ); CATCH_CHECK( 1_mg == unlib::milli<unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_g ) == typeid(             unlib::gram<VT> ) ); CATCH_CHECK( 1_g  ==              unlib::gram<VT> {1} );
		CATCH_CHECK( typeid(1_kg) == typeid(unlib::kilo <unlib::gram<VT>>) ); CATCH_CHECK( 1_kg == unlib::kilo <unlib::gram<VT>>{1} );
		CATCH_CHECK( typeid(1_t ) == typeid(             unlib:: ton<VT> ) ); CATCH_CHECK( 1_t  ==              unlib:: ton<VT> {1} );
		CATCH_CHECK( typeid(1_Mt) == typeid(unlib::mega <unlib:: ton<VT>>) ); CATCH_CHECK( 1_Mt == unlib::mega <unlib:: ton<VT>>{1} );

		CATCH_CHECK(1000_fg == 1_pg ); CATCH_CHECK(1000._fg == 1._pg );
		CATCH_CHECK(1000_pg == 1_ng ); CATCH_CHECK(1000._pg == 1._ng );
		CATCH_CHECK(1000_ng == 1_ug ); CATCH_CHECK(1000._ng == 1._ug );
		CATCH_CHECK(1000_ug == 1_mg ); CATCH_CHECK(1000._ug == 1._mg );
		CATCH_CHECK(1000_mg == 1_g  ); CATCH_CHECK(1000._mg == 1._g  );
		CATCH_CHECK(1000_g  == 1_kg ); CATCH_CHECK(1000._g  == 1._kg );
		CATCH_CHECK(1000_kg == 1_t  ); CATCH_CHECK(1000._kg == 1._t  );
		CATCH_CHECK(1000_t  == 1_kt ); CATCH_CHECK(1000._t  == 1._kt );
		CATCH_CHECK(1000_kt == 1_Mt ); CATCH_CHECK(1000._kt == 1._Mt );
	}

	CATCH_SECTION("length") {
		using VT = long long;

		const unlib::kilo<unlib::meter<VT>> km{ 1}; CATCH_REQUIRE(km.get() ==  1);

		unlib::femto<unlib::meter<VT>> fm{km}; CATCH_REQUIRE(km.get()); CATCH_CHECK(fm.get() == km.get() * 1000'000'000'000'000'000);
		unlib::pico <unlib::meter<VT>> pm{fm}; CATCH_REQUIRE(fm.get()); CATCH_CHECK(pm.get() == fm.get() / 1000);
		unlib::nano <unlib::meter<VT>> nm{pm}; CATCH_REQUIRE(pm.get()); CATCH_CHECK(nm.get() == pm.get() / 1000);
		unlib::micro<unlib::meter<VT>> um{nm}; CATCH_REQUIRE(nm.get()); CATCH_CHECK(um.get() == nm.get() / 1000);
		unlib::milli<unlib::meter<VT>> mm{um}; CATCH_REQUIRE(um.get()); CATCH_CHECK(mm.get() == um.get() / 1000);
		unlib::centi<unlib::meter<VT>> cm{mm}; CATCH_REQUIRE(mm.get()); CATCH_CHECK(cm.get() == mm.get() /   10);
		unlib::deci <unlib::meter<VT>> dm{cm}; CATCH_REQUIRE(cm.get()); CATCH_CHECK(dm.get() == cm.get() /   10);
		             unlib::meter<VT>   m{dm}; CATCH_REQUIRE(dm.get()); CATCH_CHECK( m.get() == dm.get() /   10);

		unlib::kilo <unlib::meter<VT>> k2{m};  CATCH_CHECK( k2 == km );

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1_fm) == typeid(unlib::femto<unlib::meter<VT>>) ); CATCH_CHECK( 1_fm == unlib::femto<unlib::meter<VT>>{1} );
		CATCH_CHECK( typeid(1_pm) == typeid(unlib::pico <unlib::meter<VT>>) ); CATCH_CHECK( 1_pm == unlib::pico <unlib::meter<VT>>{1} );
		CATCH_CHECK( typeid(1_nm) == typeid(unlib::nano <unlib::meter<VT>>) ); CATCH_CHECK( 1_nm == unlib::nano <unlib::meter<VT>>{1} );
		CATCH_CHECK( typeid(1_um) == typeid(unlib::micro<unlib::meter<VT>>) ); CATCH_CHECK( 1_um == unlib::micro<unlib::meter<VT>>{1} );
		CATCH_CHECK( typeid(1_mm) == typeid(unlib::milli<unlib::meter<VT>>) ); CATCH_CHECK( 1_mm == unlib::milli<unlib::meter<VT>>{1} );
		CATCH_CHECK( typeid(1_m ) == typeid(             unlib::meter<VT> ) ); CATCH_CHECK( 1_m  ==              unlib::meter<VT> {1} );
		CATCH_CHECK( typeid(1_km) == typeid(unlib::kilo <unlib::meter<VT>>) ); CATCH_CHECK( 1_km == unlib::kilo <unlib::meter<VT>>{1} );

		CATCH_CHECK(1000_fm == 1_pm ); CATCH_CHECK(1000._fm == 1._pm );
		CATCH_CHECK(1000_pm == 1_nm ); CATCH_CHECK(1000._pm == 1._nm );
		CATCH_CHECK(1000_nm == 1_um ); CATCH_CHECK(1000._nm == 1._um );
		CATCH_CHECK(1000_um == 1_mm ); CATCH_CHECK(1000._um == 1._mm );
		CATCH_CHECK(1000_mm == 1_m  ); CATCH_CHECK(1000._mm == 1._m  );
		CATCH_CHECK( 100_cm == 1_m  ); CATCH_CHECK( 100._cm == 1._m  );
		CATCH_CHECK(  10_dm == 1_m  ); CATCH_CHECK(  10._dm == 1._m  );
		CATCH_CHECK(1000_m  == 1_km ); CATCH_CHECK(1000._m  == 1._km );
	}

	CATCH_SECTION("temperature") {
		using VT = long long;

		const unlib::degree_kelvin    <VT> d_k{ 1}; CATCH_REQUIRE(d_k.get() ==  1);
		const unlib::degree_celsius   <VT> d_c{ 1}; CATCH_REQUIRE(d_c.get() ==  1);
		const unlib::degree_fahrenheit<VT> d_f{ 1}; CATCH_REQUIRE(d_f.get() ==  1);

		using namespace unlib::literals;

		CATCH_CHECK( unlib::scale_cast<unlib::degree_fahrenheit<VT>::scale_type>(unlib::degree_kelvin <VT>(10)) == unlib::degree_fahrenheit<VT>(18) );
		CATCH_CHECK( unlib::scale_cast<unlib::degree_fahrenheit<VT>::scale_type>(unlib::degree_celsius<VT>(10)) == unlib::degree_fahrenheit<VT>(18) );

	}

	CATCH_SECTION("current") {
		using VT = long long;

		const unlib::ampere<VT> A{ 1}; CATCH_REQUIRE(A.get() ==  1);

		unlib::femto<unlib::ampere<VT>> fA{ A}; CATCH_REQUIRE( A.get()); CATCH_CHECK(fA.get() ==  A.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ampere<VT>> pA{fA}; CATCH_REQUIRE(fA.get()); CATCH_CHECK(pA.get() == fA.get() / 1000);
		unlib::nano <unlib::ampere<VT>> nA{pA}; CATCH_REQUIRE(pA.get()); CATCH_CHECK(nA.get() == pA.get() / 1000);
		unlib::micro<unlib::ampere<VT>> uA{nA}; CATCH_REQUIRE(nA.get()); CATCH_CHECK(uA.get() == nA.get() / 1000);
		unlib::milli<unlib::ampere<VT>> mA{uA}; CATCH_REQUIRE(uA.get()); CATCH_CHECK(mA.get() == uA.get() / 1000);
		             unlib::ampere<VT>  A2{mA}; CATCH_REQUIRE(mA.get()); CATCH_CHECK(A2.get() ==  A.get());

		A2 *= 1000'000'000ll;
		unlib::kilo <unlib::ampere<VT>> kA{A2}; CATCH_REQUIRE(A2.get()); CATCH_CHECK(kA.get() == A2.get() / 1000);
		unlib::mega <unlib::ampere<VT>> MA{kA}; CATCH_REQUIRE(kA.get()); CATCH_CHECK(MA.get() == kA.get() / 1000);
		unlib::giga <unlib::ampere<VT>> GA{MA}; CATCH_REQUIRE(MA.get()); CATCH_CHECK(GA.get() == MA.get() / 1000);

		A2 = GA;  CATCH_CHECK( A2 == unlib::ampere<VT>{mA}*1000'000'000 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1_fA) == typeid(unlib::femto<unlib::ampere<VT>>) ); CATCH_CHECK( 1_fA == (unlib::femto<unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_pA) == typeid(unlib::pico <unlib::ampere<VT>>) ); CATCH_CHECK( 1_pA == (unlib::pico <unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_nA) == typeid(unlib::nano <unlib::ampere<VT>>) ); CATCH_CHECK( 1_nA == (unlib::nano <unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_uA) == typeid(unlib::micro<unlib::ampere<VT>>) ); CATCH_CHECK( 1_uA == (unlib::micro<unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_mA) == typeid(unlib::milli<unlib::ampere<VT>>) ); CATCH_CHECK( 1_mA == (unlib::milli<unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_A ) == typeid(             unlib::ampere<VT> ) ); CATCH_CHECK( 1_A  == (             unlib::ampere<VT> {1} ) );
		CATCH_CHECK( typeid(1_kA) == typeid(unlib::kilo <unlib::ampere<VT>>) ); CATCH_CHECK( 1_kA == (unlib::kilo <unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_MA) == typeid(unlib::mega <unlib::ampere<VT>>) ); CATCH_CHECK( 1_MA == (unlib::mega <unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_GA) == typeid(unlib::giga <unlib::ampere<VT>>) ); CATCH_CHECK( 1_GA == (unlib::giga <unlib::ampere<VT>>{1} ) );
		CATCH_CHECK( typeid(1_TA) == typeid(unlib::tera <unlib::ampere<VT>>) ); CATCH_CHECK( 1_TA == (unlib::tera <unlib::ampere<VT>>{1} ) );

		CATCH_CHECK(1000_fA == 1_pA ); CATCH_CHECK(1000._fA == 1._pA );
		CATCH_CHECK(1000_pA == 1_nA ); CATCH_CHECK(1000._pA == 1._nA );
		CATCH_CHECK(1000_nA == 1_uA ); CATCH_CHECK(1000._nA == 1._uA );
		CATCH_CHECK(1000_uA == 1_mA ); CATCH_CHECK(1000._uA == 1._mA );
		CATCH_CHECK(1000_mA == 1_A  ); CATCH_CHECK(1000._mA == 1._A  );
		CATCH_CHECK(1000_A  == 1_kA ); CATCH_CHECK(1000._A  == 1._kA );
		CATCH_CHECK(1000_kA == 1_MA ); CATCH_CHECK(1000._kA == 1._MA );
		CATCH_CHECK(1000_MA == 1_GA ); CATCH_CHECK(1000._MA == 1._GA );
		CATCH_CHECK(1000_GA == 1_TA ); CATCH_CHECK(1000._GA == 1._TA );
	}

	CATCH_SECTION("frequency") {
		using VT = long long;

		// Wikipedia: F = T^-1
		using frequency = unlib::create_unit_t< unlib::reciprocal_unit_t<unlib::time> >;
		CATCH_CHECK( typeid(frequency) == typeid(unlib::hertz<VT>::unit_type) );

		const unlib::hertz<VT> Hz{ 1}; CATCH_REQUIRE(Hz.get() ==  1);

		unlib::femto<unlib::hertz<VT>> fHz{ Hz}; CATCH_REQUIRE( Hz.get()); CATCH_CHECK(fHz.get() ==  Hz.get() * 1000'000'000'000'000);
		unlib::pico <unlib::hertz<VT>> pHz{fHz}; CATCH_REQUIRE(fHz.get()); CATCH_CHECK(pHz.get() == fHz.get() / 1000);
		unlib::nano <unlib::hertz<VT>> nHz{pHz}; CATCH_REQUIRE(pHz.get()); CATCH_CHECK(nHz.get() == pHz.get() / 1000);
		unlib::micro<unlib::hertz<VT>> uHz{nHz}; CATCH_REQUIRE(nHz.get()); CATCH_CHECK(uHz.get() == nHz.get() / 1000);
		unlib::milli<unlib::hertz<VT>> mHz{uHz}; CATCH_REQUIRE(uHz.get()); CATCH_CHECK(mHz.get() == uHz.get() / 1000);
		             unlib::hertz<VT>   H2{mHz}; CATCH_REQUIRE(mHz.get()); CATCH_CHECK( H2.get() ==  Hz.get());

		H2 *= 1000'000'000ll;
		unlib::kilo <unlib::hertz<VT>> kHz{ H2}; CATCH_REQUIRE( H2.get()); CATCH_CHECK(kHz.get() ==  H2.get() / 1000);
		unlib::mega <unlib::hertz<VT>> MHz{kHz}; CATCH_REQUIRE(kHz.get()); CATCH_CHECK(MHz.get() == kHz.get() / 1000);
		unlib::giga <unlib::hertz<VT>> GHz{MHz}; CATCH_REQUIRE(MHz.get()); CATCH_CHECK(GHz.get() == MHz.get() / 1000);

		H2 = GHz;  CATCH_CHECK( H2 == unlib::hertz<VT>{mHz}*1000'000'000 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1./1._s) == typeid(unlib::hertz<double>) );

		CATCH_CHECK( typeid(1_fHz) == typeid(unlib::femto<unlib::hertz<VT>>) ); CATCH_CHECK( 1_fHz == unlib::femto<unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_pHz) == typeid(unlib::pico <unlib::hertz<VT>>) ); CATCH_CHECK( 1_pHz == unlib::pico <unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_nHz) == typeid(unlib::nano <unlib::hertz<VT>>) ); CATCH_CHECK( 1_nHz == unlib::nano <unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_uHz) == typeid(unlib::micro<unlib::hertz<VT>>) ); CATCH_CHECK( 1_uHz == unlib::micro<unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_mHz) == typeid(unlib::milli<unlib::hertz<VT>>) ); CATCH_CHECK( 1_mHz == unlib::milli<unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_Hz ) == typeid(             unlib::hertz<VT> ) ); CATCH_CHECK( 1_Hz  ==              unlib::hertz<VT> {1} );
		CATCH_CHECK( typeid(1_kHz) == typeid(unlib::kilo <unlib::hertz<VT>>) ); CATCH_CHECK( 1_kHz == unlib::kilo <unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_MHz) == typeid(unlib::mega <unlib::hertz<VT>>) ); CATCH_CHECK( 1_MHz == unlib::mega <unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_GHz) == typeid(unlib::giga <unlib::hertz<VT>>) ); CATCH_CHECK( 1_GHz == unlib::giga <unlib::hertz<VT>>{1} );
		CATCH_CHECK( typeid(1_THz) == typeid(unlib::tera <unlib::hertz<VT>>) ); CATCH_CHECK( 1_THz == unlib::tera <unlib::hertz<VT>>{1} );

		CATCH_CHECK(1000_fHz == 1_pHz ); CATCH_CHECK(1000._fHz == 1._pHz );
		CATCH_CHECK(1000_pHz == 1_nHz ); CATCH_CHECK(1000._pHz == 1._nHz );
		CATCH_CHECK(1000_nHz == 1_uHz ); CATCH_CHECK(1000._nHz == 1._uHz );
		CATCH_CHECK(1000_uHz == 1_mHz ); CATCH_CHECK(1000._uHz == 1._mHz );
		CATCH_CHECK(1000_mHz == 1_Hz  ); CATCH_CHECK(1000._mHz == 1._Hz  );
		CATCH_CHECK(1000_Hz  == 1_kHz ); CATCH_CHECK(1000._Hz  == 1._kHz );
		CATCH_CHECK(1000_kHz == 1_MHz ); CATCH_CHECK(1000._kHz == 1._MHz );
		CATCH_CHECK(1000_MHz == 1_GHz ); CATCH_CHECK(1000._MHz == 1._GHz );
		CATCH_CHECK(1000_GHz == 1_THz ); CATCH_CHECK(1000._GHz == 1._THz );
	}

	CATCH_SECTION("voltage") {
		using VT = long long;

		// Wikipedia: U = M?L^2?I^-1?T^-3
		using voltage = unlib::create_unit_t< unlib::mass
		                                    , unlib::square_unit_t<unlib::length>
		                                    , unlib::reciprocal_unit_t<unlib::current>
		                                    , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CATCH_CHECK( typeid(voltage) == typeid(unlib::volt<VT>::unit_type) );

		const unlib::volt<VT> V{ 1}; CATCH_REQUIRE(V.get() ==  1);

		unlib::femto<unlib::volt<VT>> fV{ V}; CATCH_REQUIRE( V.get()); CATCH_CHECK(fV.get() ==  V.get() * 1000'000'000'000'000);
		unlib::pico <unlib::volt<VT>> pV{fV}; CATCH_REQUIRE(fV.get()); CATCH_CHECK(pV.get() == fV.get() / 1000);
		unlib::nano <unlib::volt<VT>> nV{pV}; CATCH_REQUIRE(pV.get()); CATCH_CHECK(nV.get() == pV.get() / 1000);
		unlib::micro<unlib::volt<VT>> uV{nV}; CATCH_REQUIRE(nV.get()); CATCH_CHECK(uV.get() == nV.get() / 1000);
		unlib::milli<unlib::volt<VT>> mV{uV}; CATCH_REQUIRE(uV.get()); CATCH_CHECK(mV.get() == uV.get() / 1000);
		             unlib::volt<VT>  V2{mV}; CATCH_REQUIRE(mV.get()); CATCH_CHECK(V2.get() ==  V.get());

		V2 *= 1000'000'000ll;
		unlib::kilo <unlib::volt<VT>> kV{V2}; CATCH_REQUIRE(V2.get()); CATCH_CHECK(kV.get() == V2.get() / 1000);
		unlib::mega <unlib::volt<VT>> MV{kV}; CATCH_REQUIRE(kV.get()); CATCH_CHECK(MV.get() == kV.get() / 1000);
		unlib::giga <unlib::volt<VT>> GV{MV}; CATCH_REQUIRE(MV.get()); CATCH_CHECK(GV.get() == MV.get() / 1000);

		V2 = GV;  CATCH_CHECK( V2 == unlib::volt<VT>{mV}*1000'000'000 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid((1._g*1._m*1._m)/(1._A*1._s*1._s*1._s)) == typeid(unlib::volt<double>) );

		CATCH_CHECK( typeid(1_fV) == typeid(unlib::femto<unlib::volt<VT>>) ); CATCH_CHECK( 1_fV == unlib::femto<unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_pV) == typeid(unlib::pico <unlib::volt<VT>>) ); CATCH_CHECK( 1_pV == unlib::pico <unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_nV) == typeid(unlib::nano <unlib::volt<VT>>) ); CATCH_CHECK( 1_nV == unlib::nano <unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_uV) == typeid(unlib::micro<unlib::volt<VT>>) ); CATCH_CHECK( 1_uV == unlib::micro<unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_mV) == typeid(unlib::milli<unlib::volt<VT>>) ); CATCH_CHECK( 1_mV == unlib::milli<unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_V ) == typeid(             unlib::volt<VT> ) ); CATCH_CHECK( 1_V  ==              unlib::volt<VT> {1} );
		CATCH_CHECK( typeid(1_kV) == typeid(unlib::kilo <unlib::volt<VT>>) ); CATCH_CHECK( 1_kV == unlib::kilo <unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_MV) == typeid(unlib::mega <unlib::volt<VT>>) ); CATCH_CHECK( 1_MV == unlib::mega <unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_GV) == typeid(unlib::giga <unlib::volt<VT>>) ); CATCH_CHECK( 1_GV == unlib::giga <unlib::volt<VT>>{1} );
		CATCH_CHECK( typeid(1_TV) == typeid(unlib::tera <unlib::volt<VT>>) ); CATCH_CHECK( 1_TV == unlib::tera <unlib::volt<VT>>{1} );

		CATCH_CHECK(1000_fV == 1_pV ); CATCH_CHECK(1000._fV == 1._pV );
		CATCH_CHECK(1000_pV == 1_nV ); CATCH_CHECK(1000._pV == 1._nV );
		CATCH_CHECK(1000_nV == 1_uV ); CATCH_CHECK(1000._nV == 1._uV );
		CATCH_CHECK(1000_uV == 1_mV ); CATCH_CHECK(1000._uV == 1._mV );
		CATCH_CHECK(1000_mV == 1_V  ); CATCH_CHECK(1000._mV == 1._V  );
		CATCH_CHECK(1000_V  == 1_kV ); CATCH_CHECK(1000._V  == 1._kV );
		CATCH_CHECK(1000_kV == 1_MV ); CATCH_CHECK(1000._kV == 1._MV );
		CATCH_CHECK(1000_MV == 1_GV ); CATCH_CHECK(1000._MV == 1._GV );
		CATCH_CHECK(1000_GV == 1_TV ); CATCH_CHECK(1000._GV == 1._TV );
	}

	CATCH_SECTION("resistance") {
		using VT = long long;

		// Wikipedia: R = M?L^2?I^-2?T^-3
		using resistance = unlib::create_unit_t< unlib::mass
		                                       , unlib::square_unit_t<unlib::length>
		                                       , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::current>>
		                                       , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CATCH_CHECK( typeid(resistance) == typeid(unlib::ohm<VT>::unit_type) );

		const unlib::ohm<VT> O{ 1}; CATCH_REQUIRE(O.get() ==  1);

		unlib::femto<unlib::ohm<VT>> fO{ O}; CATCH_REQUIRE( O.get()); CATCH_CHECK(fO.get() ==  O.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ohm<VT>> pO{fO}; CATCH_REQUIRE(fO.get()); CATCH_CHECK(pO.get() == fO.get() / 1000);
		unlib::nano <unlib::ohm<VT>> nO{pO}; CATCH_REQUIRE(pO.get()); CATCH_CHECK(nO.get() == pO.get() / 1000);
		unlib::micro<unlib::ohm<VT>> uO{nO}; CATCH_REQUIRE(nO.get()); CATCH_CHECK(uO.get() == nO.get() / 1000);
		unlib::milli<unlib::ohm<VT>> mO{uO}; CATCH_REQUIRE(uO.get()); CATCH_CHECK(mO.get() == uO.get() / 1000);
		             unlib::ohm<VT>  O2{mO}; CATCH_REQUIRE(mO.get()); CATCH_CHECK(O2.get() ==  O.get());

		O2 *= 1000'000'000ll;
		unlib::kilo <unlib::ohm<VT>> kO{O2}; CATCH_REQUIRE(O2.get()); CATCH_CHECK(kO.get() == O2.get() / 1000);
		unlib::mega <unlib::ohm<VT>> MO{kO}; CATCH_REQUIRE(kO.get()); CATCH_CHECK(MO.get() == kO.get() / 1000);
		unlib::giga <unlib::ohm<VT>> GO{MO}; CATCH_REQUIRE(MO.get()); CATCH_CHECK(GO.get() == MO.get() / 1000);

		O2 = GO;  CATCH_CHECK(O2 == unlib::ohm<VT>{mO}*1000'000'000 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1._V/1._A) == typeid(unlib::ohm<double>) );

		CATCH_CHECK( typeid(1_fO) == typeid(unlib::femto<unlib::ohm<VT>>) ); CATCH_CHECK( 1_fO == unlib::femto<unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_pO) == typeid(unlib::pico <unlib::ohm<VT>>) ); CATCH_CHECK( 1_pO == unlib::pico <unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_nO) == typeid(unlib::nano <unlib::ohm<VT>>) ); CATCH_CHECK( 1_nO == unlib::nano <unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_uO) == typeid(unlib::micro<unlib::ohm<VT>>) ); CATCH_CHECK( 1_uO == unlib::micro<unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_mO) == typeid(unlib::milli<unlib::ohm<VT>>) ); CATCH_CHECK( 1_mO == unlib::milli<unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_O ) == typeid(             unlib::ohm<VT> ) ); CATCH_CHECK( 1_O  ==              unlib::ohm<VT> {1} );
		CATCH_CHECK( typeid(1_kO) == typeid(unlib::kilo <unlib::ohm<VT>>) ); CATCH_CHECK( 1_kO == unlib::kilo <unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_MO) == typeid(unlib::mega <unlib::ohm<VT>>) ); CATCH_CHECK( 1_MO == unlib::mega <unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_GO) == typeid(unlib::giga <unlib::ohm<VT>>) ); CATCH_CHECK( 1_GO == unlib::giga <unlib::ohm<VT>>{1} );
		CATCH_CHECK( typeid(1_TO) == typeid(unlib::tera <unlib::ohm<VT>>) ); CATCH_CHECK( 1_TO == unlib::tera <unlib::ohm<VT>>{1} );

		CATCH_CHECK(1000_fO == 1_pO ); CATCH_CHECK(1000._fO == 1._pO );
		CATCH_CHECK(1000_pO == 1_nO ); CATCH_CHECK(1000._pO == 1._nO );
		CATCH_CHECK(1000_nO == 1_uO ); CATCH_CHECK(1000._nO == 1._uO );
		CATCH_CHECK(1000_uO == 1_mO ); CATCH_CHECK(1000._uO == 1._mO );
		CATCH_CHECK(1000_mO == 1_O  ); CATCH_CHECK(1000._mO == 1._O  );
		CATCH_CHECK(1000_O  == 1_kO ); CATCH_CHECK(1000._O  == 1._kO );
		CATCH_CHECK(1000_kO == 1_MO ); CATCH_CHECK(1000._kO == 1._MO );
		CATCH_CHECK(1000_MO == 1_GO ); CATCH_CHECK(1000._MO == 1._GO );
		CATCH_CHECK(1000_GO == 1_TO ); CATCH_CHECK(1000._GO == 1._TO );
	}

	CATCH_SECTION("power") {
		using VT = long long;

		using power = unlib::create_unit_t< unlib::mass
		                                  , unlib::square_unit_t<unlib::length>
		                                  , unlib::reciprocal_unit_t<unlib::cube_unit_t<unlib::time>> >;
		CATCH_CHECK( typeid(power) == typeid(unlib::watt<VT>::unit_type) );

		const unlib::      watt<VT>    W{1}; CATCH_REQUIRE(  W.get() == 1);
		const unlib::       var<VT> va_r{1}; CATCH_REQUIRE(va_r.get() == 1);
		const unlib::voltampere<VT>   va{1}; CATCH_REQUIRE( va.get() == 1);

		unlib::femto<unlib::watt<VT>> fW{ W}; CATCH_REQUIRE( W.get()); CATCH_CHECK(fW.get() ==  W.get() * 1000'000'000'000'000);
		unlib::pico <unlib::watt<VT>> pW{fW}; CATCH_REQUIRE(fW.get()); CATCH_CHECK(pW.get() == fW.get() / 1000);
		unlib::nano <unlib::watt<VT>> nW{pW}; CATCH_REQUIRE(pW.get()); CATCH_CHECK(nW.get() == pW.get() / 1000);
		unlib::micro<unlib::watt<VT>> uW{nW}; CATCH_REQUIRE(nW.get()); CATCH_CHECK(uW.get() == nW.get() / 1000);
		unlib::milli<unlib::watt<VT>> mW{uW}; CATCH_REQUIRE(uW.get()); CATCH_CHECK(mW.get() == uW.get() / 1000);

		unlib::      watt<VT>   W2{mW};                  CATCH_REQUIRE(  W2.get()); CATCH_CHECK(  W2.get() == W.get());
		unlib::       var<VT> var2{unlib::tag_cast(W2)}; CATCH_REQUIRE(var2.get()); CATCH_CHECK(var2.get() == W.get());
		unlib::voltampere<VT>  va2{unlib::tag_cast(W2)}; CATCH_REQUIRE( va2.get()); CATCH_CHECK( va2.get() == W.get());

		/*  W2 = var2; var2 =   W2; */   W2 = unlib::tag_cast(var2); var2 = unlib::tag_cast(  W2);
		/*  W2 =  va2;  va2 =   W2; */   W2 = unlib::tag_cast( va2);  va2 = unlib::tag_cast(  W2);
		/*var2 =  va2;  va2 = var2; */ var2 = unlib::tag_cast( va2);  va2 = unlib::tag_cast(var2);


		W2 *= 1000'000'000ll;
		unlib::kilo <unlib::watt<VT>> kW{W2}; CATCH_REQUIRE(W2.get()); CATCH_CHECK(kW.get() == W2.get() / 1000);
		unlib::mega <unlib::watt<VT>> MW{kW}; CATCH_REQUIRE(kW.get()); CATCH_CHECK(MW.get() == kW.get() / 1000);
		unlib::giga <unlib::watt<VT>> GW{MW}; CATCH_REQUIRE(MW.get()); CATCH_CHECK(GW.get() == MW.get() / 1000);

		 W2 = GW;  CATCH_CHECK(W2 == unlib::watt<VT>{mW}*1000'000'000 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1._V*1._A) == typeid(unlib::watt<double>) );

		CATCH_CHECK( typeid(1_fW) == typeid(unlib::femto<unlib::watt<VT>>) ); CATCH_CHECK( 1_fW == unlib::femto<unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_pW) == typeid(unlib::pico <unlib::watt<VT>>) ); CATCH_CHECK( 1_pW == unlib::pico <unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_nW) == typeid(unlib::nano <unlib::watt<VT>>) ); CATCH_CHECK( 1_nW == unlib::nano <unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_uW) == typeid(unlib::micro<unlib::watt<VT>>) ); CATCH_CHECK( 1_uW == unlib::micro<unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_mW) == typeid(unlib::milli<unlib::watt<VT>>) ); CATCH_CHECK( 1_mW == unlib::milli<unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_W ) == typeid(             unlib::watt<VT> ) ); CATCH_CHECK( 1_W  ==              unlib::watt<VT> {1} );
		CATCH_CHECK( typeid(1_kW) == typeid(unlib::kilo <unlib::watt<VT>>) ); CATCH_CHECK( 1_kW == unlib::kilo <unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_MW) == typeid(unlib::mega <unlib::watt<VT>>) ); CATCH_CHECK( 1_MW == unlib::mega <unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_GW) == typeid(unlib::giga <unlib::watt<VT>>) ); CATCH_CHECK( 1_GW == unlib::giga <unlib::watt<VT>>{1} );
		CATCH_CHECK( typeid(1_TW) == typeid(unlib::tera <unlib::watt<VT>>) ); CATCH_CHECK( 1_TW == unlib::tera <unlib::watt<VT>>{1} );

		CATCH_CHECK( typeid(1_fVAr) == typeid(unlib::femto<unlib::var<VT>>) ); CATCH_CHECK( 1_fVAr == unlib::femto<unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_pVAr) == typeid(unlib::pico <unlib::var<VT>>) ); CATCH_CHECK( 1_pVAr == unlib::pico <unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_nVAr) == typeid(unlib::nano <unlib::var<VT>>) ); CATCH_CHECK( 1_nVAr == unlib::nano <unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_uVAr) == typeid(unlib::micro<unlib::var<VT>>) ); CATCH_CHECK( 1_uVAr == unlib::micro<unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_mVAr) == typeid(unlib::milli<unlib::var<VT>>) ); CATCH_CHECK( 1_mVAr == unlib::milli<unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_VAr ) == typeid(             unlib::var<VT> ) ); CATCH_CHECK( 1_VAr  ==              unlib::var<VT> {1} );
		CATCH_CHECK( typeid(1_kVAr) == typeid(unlib::kilo <unlib::var<VT>>) ); CATCH_CHECK( 1_kVAr == unlib::kilo <unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_MVAr) == typeid(unlib::mega <unlib::var<VT>>) ); CATCH_CHECK( 1_MVAr == unlib::mega <unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_GVAr) == typeid(unlib::giga <unlib::var<VT>>) ); CATCH_CHECK( 1_GVAr == unlib::giga <unlib::var<VT>>{1} );
		CATCH_CHECK( typeid(1_TVAr) == typeid(unlib::tera <unlib::var<VT>>) ); CATCH_CHECK( 1_TVAr == unlib::tera <unlib::var<VT>>{1} );

		CATCH_CHECK( typeid(1_fVA) == typeid(unlib::femto<unlib::voltampere<VT>>) ); CATCH_CHECK( 1_fVA == unlib::femto<unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_pVA) == typeid(unlib::pico <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_pVA == unlib::pico <unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_nVA) == typeid(unlib::nano <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_nVA == unlib::nano <unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_uVA) == typeid(unlib::micro<unlib::voltampere<VT>>) ); CATCH_CHECK( 1_uVA == unlib::micro<unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_mVA) == typeid(unlib::milli<unlib::voltampere<VT>>) ); CATCH_CHECK( 1_mVA == unlib::milli<unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_VA ) == typeid(             unlib::voltampere<VT> ) ); CATCH_CHECK( 1_VA  ==              unlib::voltampere<VT> {1} );
		CATCH_CHECK( typeid(1_kVA) == typeid(unlib::kilo <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_kVA == unlib::kilo <unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_MVA) == typeid(unlib::mega <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_MVA == unlib::mega <unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_GVA) == typeid(unlib::giga <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_GVA == unlib::giga <unlib::voltampere<VT>>{1} );
		CATCH_CHECK( typeid(1_TVA) == typeid(unlib::tera <unlib::voltampere<VT>>) ); CATCH_CHECK( 1_TVA == unlib::tera <unlib::voltampere<VT>>{1} );

		CATCH_CHECK(1000_fW == 1_pW ); CATCH_CHECK(1000._fW == 1._pW );
		CATCH_CHECK(1000_pW == 1_nW ); CATCH_CHECK(1000._pW == 1._nW );
		CATCH_CHECK(1000_nW == 1_uW ); CATCH_CHECK(1000._nW == 1._uW );
		CATCH_CHECK(1000_uW == 1_mW ); CATCH_CHECK(1000._uW == 1._mW );
		CATCH_CHECK(1000_mW == 1_W  ); CATCH_CHECK(1000._mW == 1._W  );
		CATCH_CHECK(1000_W  == 1_kW ); CATCH_CHECK(1000._W  == 1._kW );
		CATCH_CHECK(1000_kW == 1_MW ); CATCH_CHECK(1000._kW == 1._MW );
		CATCH_CHECK(1000_MW == 1_GW ); CATCH_CHECK(1000._MW == 1._GW );
		CATCH_CHECK(1000_GW == 1_TW ); CATCH_CHECK(1000._GW == 1._TW );

		CATCH_CHECK(1000_VAr  == 1_kVAr ); CATCH_CHECK(1000._VAr  == 1._kVAr );
		CATCH_CHECK(1000_kVAr == 1_MVAr ); CATCH_CHECK(1000._kVAr == 1._MVAr );
		CATCH_CHECK(1000_MVAr == 1_GVAr ); CATCH_CHECK(1000._MVAr == 1._GVAr );
		CATCH_CHECK(1000_GVAr == 1_TVAr ); CATCH_CHECK(1000._GVAr == 1._TVAr );

		CATCH_CHECK(1000_VA  == 1_kVA ); CATCH_CHECK(1000._VA  == 1._kVA );
		CATCH_CHECK(1000_kVA == 1_MVA ); CATCH_CHECK(1000._kVA == 1._MVA );
		CATCH_CHECK(1000_MVA == 1_GVA ); CATCH_CHECK(1000._MVA == 1._GVA );
		CATCH_CHECK(1000_GVA == 1_TVA ); CATCH_CHECK(1000._GVA == 1._TVA );
	}

	CATCH_SECTION("energy") {
		using VT = long long;

		// Wikipedia: E = L^2?M?T^-2
		using energy = unlib::create_unit_t< unlib::square_unit_t<unlib::length>
		                                   , unlib::mass
		                                   , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
		CATCH_CHECK( typeid(energy) == typeid(unlib::energy) );
		CATCH_CHECK( typeid(energy) == typeid(unlib::watt_hour<VT>::unit_type) );

		const unlib::watt_hour  <VT> Wh{1 };           CATCH_REQUIRE(Wh.get() == 1);
		const unlib::watt_second<VT> Ws{Wh};           CATCH_REQUIRE(Ws.get() == Wh.get()*3600);
		const unlib::      joule<VT>  J{tag_cast(Ws)}; CATCH_REQUIRE( J.get() == Ws.get());

		unlib::femto<unlib::watt_second<VT>> fWs{ Ws};           CATCH_REQUIRE( Ws.get()); CATCH_CHECK(fWs.get() ==  Ws.get() * 1000'000'000'000'000);
		unlib::pico <unlib::watt_second<VT>> pWs{fWs};           CATCH_REQUIRE(fWs.get()); CATCH_CHECK(pWs.get() == fWs.get() / 1000);
		unlib::nano <unlib::watt_second<VT>> nWs{pWs};           CATCH_REQUIRE(pWs.get()); CATCH_CHECK(nWs.get() == pWs.get() / 1000);
		unlib::micro<unlib::watt_second<VT>> uWs{nWs};           CATCH_REQUIRE(nWs.get()); CATCH_CHECK(uWs.get() == nWs.get() / 1000);
		unlib::milli<unlib::watt_second<VT>> mWs{uWs};           CATCH_REQUIRE(uWs.get()); CATCH_CHECK(mWs.get() == uWs.get() / 1000);
		             unlib::watt_second<VT>  Ws2{mWs};           CATCH_REQUIRE(mWs.get()); CATCH_CHECK(Ws2.get() ==  Ws.get());
		             unlib::watt_hour  <VT>  Wh2{Ws2};           CATCH_REQUIRE(Ws2.get()); CATCH_CHECK(Ws2.get() ==  Ws.get());
		             unlib::      joule<VT>   J2{tag_cast(Ws2)}; CATCH_REQUIRE( J2.get()); CATCH_CHECK( J2.get() == Ws.get());

		Wh2 *= 1000000000ll;
		unlib::kilo <unlib::watt_hour  <VT>> kWh{Wh2}; CATCH_REQUIRE(Wh2.get()); CATCH_CHECK(kWh.get() == Wh2.get() / 1000);
		unlib::mega <unlib::watt_hour  <VT>> MWh{kWh}; CATCH_REQUIRE(kWh.get()); CATCH_CHECK(MWh.get() == kWh.get() / 1000);
		unlib::giga <unlib::watt_hour  <VT>> GWh{MWh}; CATCH_REQUIRE(MWh.get()); CATCH_CHECK(GWh.get() == MWh.get() / 1000);

		Wh2 = GWh;
		CATCH_CHECK(Wh2 == Wh*1000000000ll);

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1._W*1._h) == typeid(unlib::watt_hour<double>) );

		CATCH_CHECK( typeid(1_fWs) == typeid(unlib::femto<unlib::watt_second<VT>>) ); CATCH_CHECK( 1_fWs == unlib::femto<unlib::watt_second<VT>>{1} );
		CATCH_CHECK( typeid(1_pWs) == typeid(unlib::pico <unlib::watt_second<VT>>) ); CATCH_CHECK( 1_pWs == unlib::pico <unlib::watt_second<VT>>{1} );
		CATCH_CHECK( typeid(1_nWs) == typeid(unlib::nano <unlib::watt_second<VT>>) ); CATCH_CHECK( 1_nWs == unlib::nano <unlib::watt_second<VT>>{1} );
		CATCH_CHECK( typeid(1_uWs) == typeid(unlib::micro<unlib::watt_second<VT>>) ); CATCH_CHECK( 1_uWs == unlib::micro<unlib::watt_second<VT>>{1} );
		CATCH_CHECK( typeid(1_mWs) == typeid(unlib::milli<unlib::watt_second<VT>>) ); CATCH_CHECK( 1_mWs == unlib::milli<unlib::watt_second<VT>>{1} );
		CATCH_CHECK( typeid(1_Ws ) == typeid(             unlib::watt_second<VT> ) ); CATCH_CHECK( 1_Ws  ==              unlib::watt_second<VT> {1} );

		CATCH_CHECK( typeid(1_J  ) == typeid(             unlib::joule      <VT> ) ); CATCH_CHECK( 1_J   ==              unlib::joule      <VT> {1} );

		CATCH_CHECK( typeid(1_Wh ) == typeid(             unlib::watt_hour  <VT> ) ); CATCH_CHECK( 1_Wh  ==              unlib::watt_hour  <VT> {1} );
		CATCH_CHECK( typeid(1_kWh) == typeid(unlib::kilo <unlib::watt_hour  <VT>>) ); CATCH_CHECK( 1_kWh ==  unlib::kilo <unlib::watt_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_MWh) == typeid(unlib::mega <unlib::watt_hour  <VT>>) ); CATCH_CHECK( 1_MWh ==  unlib::mega <unlib::watt_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_GWh) == typeid(unlib::giga <unlib::watt_hour  <VT>>) ); CATCH_CHECK( 1_GWh ==  unlib::giga <unlib::watt_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_TWh) == typeid(unlib::tera <unlib::watt_hour  <VT>>) ); CATCH_CHECK( 1_TWh ==  unlib::tera <unlib::watt_hour  <VT>>{1} );

		CATCH_CHECK(1_Ws == unlib::watt_second<VT>{unlib::tag_cast(1_J)}); CATCH_CHECK(1._Ws == unlib::watt_second<double>{unlib::tag_cast(1._J)});

		CATCH_CHECK(1000_fWs == 1_pWs ); CATCH_CHECK(1000._fWs == 1._pWs );
		CATCH_CHECK(1000_pWs == 1_nWs ); CATCH_CHECK(1000._pWs == 1._nWs );
		CATCH_CHECK(1000_nWs == 1_uWs ); CATCH_CHECK(1000._nWs == 1._uWs );
		CATCH_CHECK(1000_uWs == 1_mWs ); CATCH_CHECK(1000._uWs == 1._mWs );
		CATCH_CHECK(1000_mWs == 1_Ws  ); CATCH_CHECK(1000._mWs == 1._Ws  );
		CATCH_CHECK(3600_Ws  == 1_Wh  ); CATCH_CHECK(3600._Ws  == 1._Wh  );
		CATCH_CHECK(1000_Wh  == 1_kWh ); CATCH_CHECK(1000._Wh  == 1._kWh );
		CATCH_CHECK(1000_kWh == 1_MWh ); CATCH_CHECK(1000._kWh == 1._MWh );
		CATCH_CHECK(1000_MWh == 1_GWh ); CATCH_CHECK(1000._MWh == 1._GWh );
		CATCH_CHECK(1000_GWh == 1_TWh ); CATCH_CHECK(1000._GWh == 1._TWh );
	}

	CATCH_SECTION("electric charge") {
		using VT = long long;

		// Wikipedia: Q = I?T
		using charge = unlib::create_unit_t< unlib::current
		                                   , unlib::time >;
		CATCH_CHECK( typeid(charge) == typeid(unlib::ampere_hour<VT>::unit_type) );

		const unlib::ampere_hour  <VT> Ah{1 }; CATCH_REQUIRE(Ah.get() == 1);
		const unlib::ampere_second<VT> As{Ah}; CATCH_REQUIRE(As.get() == Ah.get()*3600);

		unlib::femto<unlib::ampere_second<VT>> fAs{ As}; CATCH_REQUIRE( As.get()); CATCH_CHECK(fAs.get() ==  As.get() * 1000'000'000'000'000);
		unlib::pico <unlib::ampere_second<VT>> pAs{fAs}; CATCH_REQUIRE(fAs.get()); CATCH_CHECK(pAs.get() == fAs.get() / 1000);
		unlib::nano <unlib::ampere_second<VT>> nAs{pAs}; CATCH_REQUIRE(pAs.get()); CATCH_CHECK(nAs.get() == pAs.get() / 1000);
		unlib::micro<unlib::ampere_second<VT>> uAs{nAs}; CATCH_REQUIRE(nAs.get()); CATCH_CHECK(uAs.get() == nAs.get() / 1000);
		unlib::milli<unlib::ampere_second<VT>> mAs{uAs}; CATCH_REQUIRE(uAs.get()); CATCH_CHECK(mAs.get() == uAs.get() / 1000);
		             unlib::ampere_second<VT>  As2{mAs}; CATCH_REQUIRE(mAs.get()); CATCH_CHECK(As2.get() ==  As.get());

		             unlib::ampere_hour  <VT>  Ah2{As2}; CATCH_REQUIRE(As2.get()); CATCH_CHECK(As2.get() ==  As.get());
		Ah2 *= 1000000000ll;
		unlib::kilo <unlib::ampere_hour  <VT>> kAh{Ah2}; CATCH_REQUIRE(Ah2.get()); CATCH_CHECK(kAh.get() == Ah2.get() / 1000);
		unlib::mega <unlib::ampere_hour  <VT>> MAh{kAh}; CATCH_REQUIRE(kAh.get()); CATCH_CHECK(MAh.get() == kAh.get() / 1000);
		unlib::giga <unlib::ampere_hour  <VT>> GAh{MAh}; CATCH_REQUIRE(MAh.get()); CATCH_CHECK(GAh.get() == MAh.get() / 1000);

		Ah2 = GAh;
		CATCH_CHECK(Ah2 == Ah*1000000000ll);

		using namespace unlib::literals;

		CATCH_CHECK( typeid(1._A*1._h) == typeid(unlib::ampere_hour<double>) );

		CATCH_CHECK( typeid(1_fAs) == typeid(unlib::femto<unlib::ampere_second<VT>>) ); CATCH_CHECK( 1_fAs == unlib::femto<unlib::ampere_second<VT>>{1} );
		CATCH_CHECK( typeid(1_pAs) == typeid(unlib::pico <unlib::ampere_second<VT>>) ); CATCH_CHECK( 1_pAs == unlib::pico <unlib::ampere_second<VT>>{1} );
		CATCH_CHECK( typeid(1_nAs) == typeid(unlib::nano <unlib::ampere_second<VT>>) ); CATCH_CHECK( 1_nAs == unlib::nano <unlib::ampere_second<VT>>{1} );
		CATCH_CHECK( typeid(1_uAs) == typeid(unlib::micro<unlib::ampere_second<VT>>) ); CATCH_CHECK( 1_uAs == unlib::micro<unlib::ampere_second<VT>>{1} );
		CATCH_CHECK( typeid(1_mAs) == typeid(unlib::milli<unlib::ampere_second<VT>>) ); CATCH_CHECK( 1_mAs == unlib::milli<unlib::ampere_second<VT>>{1} );
		CATCH_CHECK( typeid(1_As ) == typeid(             unlib::ampere_second<VT> ) ); CATCH_CHECK( 1_As  ==              unlib::ampere_second<VT> {1} );

		CATCH_CHECK( typeid(1_Ah ) == typeid(             unlib::ampere_hour  <VT> ) ); CATCH_CHECK( 1_Ah  ==              unlib::ampere_hour  <VT> {1} );
		CATCH_CHECK( typeid(1_kAh) == typeid(unlib::kilo <unlib::ampere_hour  <VT>>) ); CATCH_CHECK( 1_kAh == unlib::kilo <unlib::ampere_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_MAh) == typeid(unlib::mega <unlib::ampere_hour  <VT>>) ); CATCH_CHECK( 1_MAh == unlib::mega <unlib::ampere_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_GAh) == typeid(unlib::giga <unlib::ampere_hour  <VT>>) ); CATCH_CHECK( 1_GAh == unlib::giga <unlib::ampere_hour  <VT>>{1} );
		CATCH_CHECK( typeid(1_TAh) == typeid(unlib::tera <unlib::ampere_hour  <VT>>) ); CATCH_CHECK( 1_TAh == unlib::tera <unlib::ampere_hour  <VT>>{1} );

		CATCH_CHECK(1000_fAs == 1_pAs ); CATCH_CHECK(1000._fAs == 1._pAs );
		CATCH_CHECK(1000_pAs == 1_nAs ); CATCH_CHECK(1000._pAs == 1._nAs );
		CATCH_CHECK(1000_nAs == 1_uAs ); CATCH_CHECK(1000._nAs == 1._uAs );
		CATCH_CHECK(1000_uAs == 1_mAs ); CATCH_CHECK(1000._uAs == 1._mAs );
		CATCH_CHECK(1000_mAs == 1_As  ); CATCH_CHECK(1000._mAs == 1._As  );
		CATCH_CHECK(3600_As  == 1_Ah  ); CATCH_CHECK(3600._As  == 1._Ah  );
		CATCH_CHECK(1000_Ah  == 1_kAh ); CATCH_CHECK(1000._Ah  == 1._kAh );
		CATCH_CHECK(1000_kAh == 1_MAh ); CATCH_CHECK(1000._kAh == 1._MAh );
		CATCH_CHECK(1000_MAh == 1_GAh ); CATCH_CHECK(1000._MAh == 1._GAh );
		CATCH_CHECK(1000_GAh == 1_TAh ); CATCH_CHECK(1000._GAh == 1._TAh );
	}

	CATCH_SECTION("pressure") {
		using VT = long long;

		// Wikipedia: P =  	M?L^-1?T^-2
		using pressure = unlib::create_unit_t< unlib::mass
		                                     , unlib::reciprocal_unit_t<unlib::length>
		                                     , unlib::reciprocal_unit_t<unlib::square_unit_t<unlib::time>> >;
		CATCH_CHECK( typeid(pressure) == typeid(unlib::pressure) );
		CATCH_CHECK( typeid(pressure) == typeid(unlib::bar<VT>::unit_type) );

		const unlib::    bar<VT> bar1{1   }; CATCH_REQUIRE(bar1.get() == 1);
		const unlib::pascal_<VT>   Pa{bar1}; CATCH_REQUIRE(  Pa.get() == bar1.get()*100000);

		unlib::femto<unlib::bar    <VT>> fbar{bar1}; CATCH_REQUIRE(bar1.get()); CATCH_CHECK(fbar.get() == bar1.get() * 1000'000'000'000'000);
		unlib::pico <unlib::bar    <VT>> pbar{fbar}; CATCH_REQUIRE(fbar.get()); CATCH_CHECK(pbar.get() == fbar.get() / 1000);
		unlib::nano <unlib::bar    <VT>> nbar{pbar}; CATCH_REQUIRE(pbar.get()); CATCH_CHECK(nbar.get() == pbar.get() / 1000);
		unlib::micro<unlib::bar    <VT>> ubar{nbar}; CATCH_REQUIRE(nbar.get()); CATCH_CHECK(ubar.get() == nbar.get() / 1000);
		unlib::milli<unlib::bar    <VT>> mbar{ubar}; CATCH_REQUIRE(ubar.get()); CATCH_CHECK(mbar.get() == ubar.get() / 1000);
		             unlib::bar    <VT>  bar2{mbar}; CATCH_REQUIRE(mbar.get()); CATCH_CHECK(bar2.get() == bar1.get());

		             unlib::pascal_<VT>   Pa2{bar2}; CATCH_REQUIRE(bar2.get()); CATCH_CHECK(bar2.get() == bar1.get());

		Pa2 *= 1000000000ll;
		unlib::kilo <unlib::pascal_<VT>> kPa{Pa2}; CATCH_REQUIRE(Pa2.get()); CATCH_CHECK(kPa.get() == Pa2.get() / 1000);
		unlib::hecto<unlib::pascal_<VT>> hPa{Pa2}; CATCH_REQUIRE(Pa2.get()); CATCH_CHECK(hPa.get() == Pa2.get() / 100 );
		unlib::mega <unlib::pascal_<VT>> MPa{kPa}; CATCH_REQUIRE(kPa.get()); CATCH_CHECK(MPa.get() == kPa.get() / 1000);
		unlib::giga <unlib::pascal_<VT>> GPa{MPa}; CATCH_REQUIRE(MPa.get()); CATCH_CHECK(GPa.get() == MPa.get() / 1000);

		Pa2 = GPa;
		CATCH_CHECK(Pa2 == Pa*1000000000ll);

		using namespace unlib::literals;

		CATCH_CHECK( typeid( 1._g / (1._m * 1._s * 1._s)) == typeid(unlib::pascal_<double>) );

		CATCH_CHECK( typeid(1_pbar) == typeid(unlib::pico <unlib::bar<VT>>) ); CATCH_CHECK( 1_pbar == unlib::pico <unlib::bar<VT>>{1} );
		CATCH_CHECK( typeid(1_nbar) == typeid(unlib::nano <unlib::bar<VT>>) ); CATCH_CHECK( 1_nbar == unlib::nano <unlib::bar<VT>>{1} );
		CATCH_CHECK( typeid(1_ubar) == typeid(unlib::micro<unlib::bar<VT>>) ); CATCH_CHECK( 1_ubar == unlib::micro<unlib::bar<VT>>{1} );
		CATCH_CHECK( typeid(1_mbar) == typeid(unlib::milli<unlib::bar<VT>>) ); CATCH_CHECK( 1_mbar == unlib::milli<unlib::bar<VT>>{1} );
		CATCH_CHECK( typeid(1_bar ) == typeid(             unlib::bar<VT> ) ); CATCH_CHECK( 1_bar  ==              unlib::bar<VT> {1} );

		CATCH_CHECK( typeid(1_Pa ) == typeid(             unlib::pascal_<VT> ) ); CATCH_CHECK( 1_Pa  ==              unlib::pascal_<VT> {1} );
		CATCH_CHECK( typeid(1_hPa) == typeid(unlib::hecto<unlib::pascal_<VT>>) ); CATCH_CHECK( 1_hPa == unlib::hecto<unlib::pascal_<VT>>{1} );
		CATCH_CHECK( typeid(1_kPa) == typeid(unlib::kilo <unlib::pascal_<VT>>) ); CATCH_CHECK( 1_kPa == unlib::kilo <unlib::pascal_<VT>>{1} );
		CATCH_CHECK( typeid(1_MPa) == typeid(unlib::mega <unlib::pascal_<VT>>) ); CATCH_CHECK( 1_MPa == unlib::mega <unlib::pascal_<VT>>{1} );
		CATCH_CHECK( typeid(1_GPa) == typeid(unlib::giga <unlib::pascal_<VT>>) ); CATCH_CHECK( 1_GPa == unlib::giga <unlib::pascal_<VT>>{1} );
		CATCH_CHECK( typeid(1_TPa) == typeid(unlib::tera <unlib::pascal_<VT>>) ); CATCH_CHECK( 1_TPa == unlib::tera <unlib::pascal_<VT>>{1} );

		CATCH_CHECK(1000_pbar == 1_nbar ); CATCH_CHECK(1000._pbar == 1._nbar );
		CATCH_CHECK(1000_nbar == 1_ubar ); CATCH_CHECK(1000._nbar == 1._ubar );
		CATCH_CHECK(1000_ubar == 1_mbar ); CATCH_CHECK(1000._ubar == 1._mbar );
		CATCH_CHECK(1000_mbar == 1_bar  ); CATCH_CHECK(1000._mbar == 1._bar  );
		CATCH_CHECK( 100_kPa  == 1_bar  ); CATCH_CHECK( 100._kPa  == 1._bar  );
		CATCH_CHECK( 100_Pa   == 1_hPa  ); CATCH_CHECK( 100._Pa   == 1._hPa  );
		CATCH_CHECK(1000_Pa   == 1_kPa  ); CATCH_CHECK(1000._Pa   == 1._kPa  );
		CATCH_CHECK(1000_kPa  == 1_MPa  ); CATCH_CHECK(1000._kPa  == 1._MPa  );
		CATCH_CHECK(1000_MPa  == 1_GPa  ); CATCH_CHECK(1000._MPa  == 1._GPa  );
		CATCH_CHECK(1000_GPa  == 1_TPa  ); CATCH_CHECK(1000._GPa  == 1._TPa  );
	}

	CATCH_SECTION("velocity") {
		using VT = long long;

		// Wikipedia: a =  	L?T^-1
		using velocity = unlib::create_unit_t< unlib::length
		                                     , unlib::reciprocal_unit_t<unlib::time> >;
		CATCH_CHECK( typeid(            velocity ) == typeid(unlib::velocity) );
		CATCH_CHECK( typeid(unlib::    meter_per_second<VT>::unit_type) == typeid(unlib::velocity) );
		CATCH_CHECK( typeid(unlib::kilometer_per_hour  <VT>::unit_type) == typeid(unlib::velocity) );

		const unlib::    meter_per_second<VT> mps {100}; CATCH_REQUIRE( mps.get() == 100             );
		const unlib::kilometer_per_hour  <VT> kmph{mps}; CATCH_CHECK(kmph.get()   == mps.get()*36/10 );

		using namespace unlib::literals;

		CATCH_CHECK( typeid( 1._m  / 1._s) == typeid(unlib::  meter_per_second<double>) );
		CATCH_CHECK( typeid( 1._km / 1._h) == typeid(unlib::kilometer_per_hour<double>) );

		CATCH_CHECK( typeid(1_m_per_s ) == typeid(unlib::    meter_per_second<VT>) ); CATCH_CHECK( 1_m_per_s  == unlib::    meter_per_second<VT>{1} );
		CATCH_CHECK( typeid(1_km_per_h) == typeid(unlib::kilometer_per_hour  <VT>) ); CATCH_CHECK( 1_km_per_h == unlib::kilometer_per_hour  <VT>{1} );

		CATCH_CHECK(10_m_per_s == 36_km_per_h); CATCH_CHECK(1.0_m_per_s == 3.6_km_per_h);
	}
}
