#include <unlib/quantity.hpp>
#include <string>
#include <functional>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

namespace {

using default_test_unit = unlib::unit_t<unlib::pow_unit_t<unlib::            time,std::ratio<1>>
                                       ,unlib::pow_unit_t<unlib::            mass,std::ratio<2>>
                                       ,unlib::pow_unit_t<unlib::          length,std::ratio<3>>
                                       ,unlib::pow_unit_t<unlib::         current,std::ratio<4>>
                                       ,unlib::pow_unit_t<unlib::      luminosity,std::ratio<5>>
                                       ,unlib::pow_unit_t<unlib::     temperature,std::ratio<6>>
                                       ,unlib::pow_unit_t<unlib::substance_amount,std::ratio<7>> >;

}

TEST_CASE("quantities") {

SUBCASE("quantities type operations") {
	using namespace unlib;
	using scale_type = scale_t<1,2>;
	using value_type = double;
	using test_quantity = quantity<default_test_unit, scale_type, value_type>;

	SUBCASE("template parameters properly propagate to quantity") {
		CHECK( typeid(default_test_unit) == typeid(test_quantity::unit_type)  );
		CHECK( typeid(value_type)        == typeid(test_quantity::value_type) );

		CHECK( typeid(scale_type)        == typeid(test_quantity::scale_type) );
		CHECK( typeid(no_tag)            == typeid(test_quantity::tag_type)   );

		CHECK( typeid(exponent_t<1>)       == typeid(test_quantity::            time_exponent) );
		CHECK( typeid(exponent_t<2>)       == typeid(test_quantity::            mass_exponent) );
		CHECK( typeid(exponent_t<3>)       == typeid(test_quantity::          length_exponent) );
		CHECK( typeid(exponent_t<4>)       == typeid(test_quantity::         current_exponent) );
		CHECK( typeid(exponent_t<5>)       == typeid(test_quantity::      luminosity_exponent) );
		CHECK( typeid(exponent_t<6>)       == typeid(test_quantity::     temperature_exponent) );
		CHECK( typeid(exponent_t<7>)       == typeid(test_quantity::substance_amount_exponent) );
	}

	SUBCASE("quantities can be retagged") {
		using test_q_foo = test_quantity::retag<tag_t<struct foo_tag_id>>;
		using test_q_bar = test_q_foo   ::retag<tag_t<struct bar_tag_id>>;

		REQUIRE( typeid(test_quantity::tag_type) == typeid(unlib::no_tag         ) );
		CHECK  ( typeid(test_q_foo::tag_type)    == typeid(tag_t<struct foo_tag_id>) );
		CHECK  ( typeid(test_q_bar::tag_type)    == typeid(tag_t<struct bar_tag_id>) );
	}

	SUBCASE("quantities can be rescaled") {
		using test_quantity_a = test_quantity;
		REQUIRE( typeid(test_quantity_a::scale_type)                             == typeid(scale_t< 1, 2>) );
		CHECK  ( typeid(test_quantity_a::rescale_to<scale_t<47,11>>::scale_type) == typeid(scale_t<47,11>) );
		CHECK  ( typeid(test_quantity_a::rescale_by<scale_t< 4, 1>>::scale_type) == typeid(scale_t< 2, 1>) );

		using test_quantity_b = unlib::quantity<default_test_unit>;
		CHECK( typeid( to_atto <test_quantity_b> ) == typeid( quantity<default_test_unit,  atto_scaling> ) );
		CHECK( typeid( to_femto<test_quantity_b> ) == typeid( quantity<default_test_unit, femto_scaling> ) );
		CHECK( typeid( to_pico <test_quantity_b> ) == typeid( quantity<default_test_unit,  pico_scaling> ) );
		CHECK( typeid( to_nano <test_quantity_b> ) == typeid( quantity<default_test_unit,  nano_scaling> ) );
		CHECK( typeid( to_micro<test_quantity_b> ) == typeid( quantity<default_test_unit, micro_scaling> ) );
		CHECK( typeid( to_milli<test_quantity_b> ) == typeid( quantity<default_test_unit, milli_scaling> ) );
		CHECK( typeid( to_centi<test_quantity_b> ) == typeid( quantity<default_test_unit, centi_scaling> ) );
		CHECK( typeid( to_deci <test_quantity_b> ) == typeid( quantity<default_test_unit,  deci_scaling> ) );
		CHECK( typeid( to_deca <test_quantity_b> ) == typeid( quantity<default_test_unit,  deca_scaling> ) );
		CHECK( typeid( to_hecto<test_quantity_b> ) == typeid( quantity<default_test_unit, hecto_scaling> ) );
		CHECK( typeid( to_kilo <test_quantity_b> ) == typeid( quantity<default_test_unit,  kilo_scaling> ) );
		CHECK( typeid( to_mega <test_quantity_b> ) == typeid( quantity<default_test_unit,  mega_scaling> ) );
		CHECK( typeid( to_giga <test_quantity_b> ) == typeid( quantity<default_test_unit,  giga_scaling> ) );
		CHECK( typeid( to_tera <test_quantity_b> ) == typeid( quantity<default_test_unit,  tera_scaling> ) );
		CHECK( typeid( to_peta <test_quantity_b> ) == typeid( quantity<default_test_unit,  peta_scaling> ) );
		CHECK( typeid( to_exa  <test_quantity_b> ) == typeid( quantity<default_test_unit,   exa_scaling> ) );

		CHECK( typeid( to_milli<to_kilo<test_quantity_b>> ) == typeid( to_milli<test_quantity_b> ) );
	}
}

SUBCASE("quantity values") {
	using namespace unlib;

	using value_type = double;
	using test_quantity = quantity<default_test_unit, no_scaling, value_type>;

	SUBCASE("quantity is default-initialized") {
		{
			test_quantity q{1000}; // make stack memory dirty
		}
		{
			test_quantity q;
			CHECK(q.is_near_zero()); CHECK(is_near_zero(q));
		}
	}
	SUBCASE("value is stored correctly") {
		const value_type v = 42.;

		const test_quantity q1{v};
		CHECK( v == doctest::Approx(q1.get()) );
		const test_quantity q2(q1);
		CHECK( v == doctest::Approx(q2.get()) );
		test_quantity q3;
		CHECK( q3.is_near_zero() );     CHECK( is_near_zero(q3) );
		q3 = q1;
		CHECK( not q3.is_near_zero() ); CHECK( not is_near_zero(q3) );
		CHECK( v == doctest::Approx(q3.get()) );
	}

	SUBCASE("is_near_zero") {
		using test_q = test_quantity::rescale_to<scale_t<1,2>>;

		const test_q q1{};
		CHECK( q1.is_near_zero() ); CHECK( is_near_zero(q1) );

		const value_type e1 = 101*std::numeric_limits<float>::epsilon();
		const test_q q2{e1};
		CHECK( not q2.is_near_zero()   ); CHECK( not is_near_zero(q2)         );
		CHECK(     q2.is_near_zero(e1) ); CHECK(     is_near_zero(q2,e1) );

		const value_type e2 = 100*std::numeric_limits<float>::epsilon();
		const test_q q3{e2};
		CHECK( q3.is_near_zero()   ); CHECK( is_near_zero(q3)    );
		CHECK( q3.is_near_zero(e1) ); CHECK( is_near_zero(q3,e1) );
		CHECK( q3.is_near_zero(e2) ); CHECK( is_near_zero(q3,e2) );

		const value_type e3 = 99*std::numeric_limits<float>::epsilon();
		const test_q q4{e3};
		CHECK( q4.is_near_zero()   ); CHECK( is_near_zero(q4)    );
		CHECK( q4.is_near_zero(e1) ); CHECK( is_near_zero(q4,e1) );
		CHECK( q4.is_near_zero(e2) ); CHECK( is_near_zero(q4,e2) );
		CHECK( q4.is_near_zero(e3) ); CHECK( is_near_zero(q4,e3) );
	}

	SUBCASE("values are rescaled properly") {
		const int original_value = 42000;
		int downscaled_value = detail::rescale_value<milli_scaling, no_scaling>(original_value);
		CHECK( downscaled_value == 1000*original_value );

		int upscaled_value = detail::rescale_value< kilo_scaling, no_scaling >(original_value);
		CHECK( upscaled_value == original_value/1000 );

		struct test_value_type {
			bool multiplication_called{false};

			test_value_type operator*(test_value_type)                        {multiplication_called=true; return *this;}
		};

		test_value_type test_value;
		detail::rescale_value<    no_scaling,    no_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  deci_scaling,  deci_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< centi_scaling, centi_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< milli_scaling, milli_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< micro_scaling, micro_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  kilo_scaling,  kilo_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  mega_scaling,  mega_scaling >(test_value); CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
	}

	SUBCASE("values can be re-scaled implicitly through constructors") {
		const value_type v = 42;

		         test_quantity   q1{  v*1000000000000000000}; CHECK( v*1000000000000000000 == doctest::Approx( q1.get()) );
		to_deca <test_quantity> daq{ q1                    }; CHECK( v*100000000000000000  == doctest::Approx(daq.get()) );
		to_hecto<test_quantity>  hq{daq                    }; CHECK( v*10000000000000000   == doctest::Approx( hq.get()) );
		to_kilo <test_quantity>  kq{ hq                    }; CHECK( v*1000000000000000    == doctest::Approx( kq.get()) );
		to_mega <test_quantity>  Mq{ kq                    }; CHECK( v*1000000000000       == doctest::Approx( Mq.get()) );
		to_giga <test_quantity>  Gq{ Mq                    }; CHECK( v*1000000000          == doctest::Approx( Gq.get()) );

//		to_exa  <test_quantity>  Eq{ v }; CHECK( v                     == doctest::Approx( Eq.get()) );
//		to_peta <test_quantity>  Pq{ Eq}; CHECK( v*1000                == doctest::Approx( Pq.get()) );
//		to_tera <test_quantity>  Tq{ Pq}; CHECK( v*1000000             == doctest::Approx( Tq.get()) );
//		to_giga <test_quantity>  Gq{ Tq}; CHECK( v*1000000000          == doctest::Approx( Gq.get()) );
//		to_mega <test_quantity>  Mq{ Gq}; CHECK( v*1000000000000       == doctest::Approx( Mq.get()) );
//		to_kilo <test_quantity>  kq{ Mq}; CHECK( v*1000000000000000    == doctest::Approx( kq.get()) );
//		to_hecto<test_quantity>  hq{ kq}; CHECK( v*10000000000000000   == doctest::Approx( hq.get()) );
//		to_deca <test_quantity> daq{ hq}; CHECK( v*100000000000000000  == doctest::Approx(daq.get()) );
//		         test_quantity   q1{daq}; CHECK( v*1000000000000000000 == doctest::Approx( q1.get()) );
		         test_quantity   q2{ v }; CHECK( v                     == doctest::Approx( q2.get()) );
		to_deci <test_quantity>  dq{ q2}; CHECK( v*10                  == doctest::Approx( dq.get()) );
		to_centi<test_quantity>  cq{ dq}; CHECK( v*100                 == doctest::Approx( cq.get()) );
		to_milli<test_quantity>  mq{ dq}; CHECK( v*1000                == doctest::Approx( mq.get()) );
		to_micro<test_quantity>  uq{ mq}; CHECK( v*1000000             == doctest::Approx( uq.get()) );
		to_nano <test_quantity>  nq{ uq}; CHECK( v*1000000000          == doctest::Approx( nq.get()) );
//		to_pico <test_quantity>  pq{ nq}; CHECK( v*1000000000000       == doctest::Approx( pq.get()) );
//		to_femto<test_quantity>  fq{ pq}; CHECK( v*1000000000000000    == doctest::Approx( fq.get()) );
//		to_atto <test_quantity>  aq{ fq}; CHECK( v*1000000000000000000 == doctest::Approx( aq.get()) );
	}

	SUBCASE("values can be re-scaled through get") {
		const value_type v = 42000;

		test_quantity q1{v};
		CHECK( v*1000  == doctest::Approx(q1.get_scaled<milli_scaling>()) );

		test_quantity q2{v};
		CHECK( v/1000. == doctest::Approx(q2.get_scaled<kilo_scaling>()) );
	}

	SUBCASE("quantities can be compared") {
		using test_mq = test_quantity::rescale_to<milli_scaling>;

		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{v};
		test_quantity q3{v*2};
		test_mq mq1{q1};

		CHECK( is_near(q1,q1) );
		CHECK( is_near(q1,q2) );
		CHECK( is_near(q2,q1) );
		CHECK( is_near(q2,q2) );

		CHECK( q1 <= q2 );
		CHECK( q1 >= q2 );
		CHECK( q2 <= q1 );
		CHECK( q2 >= q1 );

		CHECK( not is_near(q1,q3) );
		CHECK( not is_near(q3,q1) );

		CHECK( q1 <  q3 );
		CHECK( q3 >  q1 );
		CHECK( q1 <= q3 );
		CHECK( q3 >= q1 );

		CHECK( is_near(mq1, q1) );
		CHECK( is_near( q1,mq1) );
		CHECK( is_near(mq1, q2) );
		CHECK( is_near( q2,mq1) );

		CHECK( mq1 <=  q2 );
		CHECK( mq1 >=  q2 );
		CHECK(  q2 <= mq1 );
		CHECK(  q2 >= mq1 );

		CHECK( not is_near(mq1, q3) );
		CHECK( not is_near( q3,mq1) );

		CHECK( mq1 <   q3 );
		CHECK(  q3 >  mq1 );
		CHECK( mq1 <=  q3 );
		CHECK(  q3 >= mq1 );
	}

	SUBCASE("is_near") {
		using local_value_type = double;
		using test_q = test_quantity::rescale_to<scale_t<1,2>>;

		const local_value_type e1 = 50*std::numeric_limits<float>::epsilon();

		const test_q q0{}, q1{e1}, q2{42}, q3{42}, q4{42 + e1}, q5{2 * 42.};

		CHECK(     q0.is_near(q1) ); CHECK(     is_near(q0,q1) );
		CHECK( not q0.is_near(q2) ); CHECK( not is_near(q0,q2) );
		CHECK( not q0.is_near(q3) ); CHECK( not is_near(q0,q3) );
		CHECK( not q0.is_near(q4) ); CHECK( not is_near(q0,q4) );
		CHECK( not q0.is_near(q5) ); CHECK( not is_near(q0,q5) );

		CHECK( not q1.is_near(q2) ); CHECK( not is_near(q1,q2) );
		CHECK( not q1.is_near(q3) ); CHECK( not is_near(q1,q3) );
		CHECK( not q1.is_near(q3) ); CHECK( not is_near(q1,q3) );
		CHECK( not q1.is_near(q4) ); CHECK( not is_near(q1,q4) );
		CHECK( not q1.is_near(q5) ); CHECK( not is_near(q1,q5) );

		CHECK(     q2.is_near(q3) ); CHECK(     is_near(q2,q3) );
		CHECK(     q2.is_near(q4) ); CHECK(     is_near(q2,q4) );
		CHECK( not q2.is_near(q5) ); CHECK( not is_near(q2,q5) );

		CHECK(     q3.is_near(q4) ); CHECK(     is_near(q3,q4) );
		CHECK( not q3.is_near(q5) ); CHECK( not is_near(q3,q5) );

		CHECK( not q4.is_near(q5) ); CHECK( not is_near(q4,q5) );
	}

	SUBCASE("quantities can be signed") {
		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{-v};

		CHECK(q1.is_near(+q1));
		CHECK(q2.is_near(-q1));
		CHECK(q1.is_near(-q2));
	}
}

SUBCASE("quantity mathematical operations") {
	using namespace unlib;

	using value_type = double;
	using test_quantity  = quantity<default_test_unit, no_scaling, value_type>;

	SUBCASE("quantities can be added and subtracted") {
		using test_mq = test_quantity::rescale_to<milli_scaling>;

		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{v*2};
		test_quantity q3{v};
		q1 += q2;
		q3 += q1;

		CHECK( q1.get() == doctest::Approx(3*v) );
		CHECK( q2.get() == doctest::Approx(2*v) );
		CHECK( q3.get() == doctest::Approx(4*v) );

		CHECK( (q1+q2).get() == doctest::Approx(5*v) );

		test_mq mqu1{q1};
		mqu1 += q1;
		CHECK( mqu1.get() == doctest::Approx(6000*v) );

		q1 += mqu1;
		CHECK( q1.get() == doctest::Approx(9*v) );


		q1 -= mqu1;
		CHECK( q1.get() == doctest::Approx(3*v) );

		q1 -= q2;
		CHECK( q1.get() == doctest::Approx(v) );

		CHECK( (q2-q1).get() == doctest::Approx(v) );
	}

	SUBCASE("quantities can be multiplied and divided") {
		using test_unit_b   = unit_t<pow_unit_t<unlib::            time,std::ratio<7>>
		                            ,pow_unit_t<unlib::            mass,std::ratio<6>>
		                            ,pow_unit_t<unlib::          length,std::ratio<5>>
		                            ,pow_unit_t<unlib::         current,std::ratio<4>>
		                            ,pow_unit_t<unlib::      luminosity,std::ratio<3>>
		                            ,pow_unit_t<unlib::     temperature,std::ratio<2>>
		                            ,pow_unit_t<unlib::substance_amount,std::ratio<1>> >;
		using test_unit_mab = unit_t<pow_unit_t<unlib::            time,std::ratio<8>>
		                            ,pow_unit_t<unlib::            mass,std::ratio<8>>
		                            ,pow_unit_t<unlib::          length,std::ratio<8>>
		                            ,pow_unit_t<unlib::         current,std::ratio<8>>
		                            ,pow_unit_t<unlib::      luminosity,std::ratio<8>>
		                            ,pow_unit_t<unlib::     temperature,std::ratio<8>>
		                            ,pow_unit_t<unlib::substance_amount,std::ratio<8>> >;
		using test_unit_dab = unit_t<pow_unit_t<unlib::            time,std::ratio<-6>>
		                            ,pow_unit_t<unlib::            mass,std::ratio<-4>>
		                            ,pow_unit_t<unlib::          length,std::ratio<-2>>
		                            ,pow_unit_t<unlib::         current,std::ratio<0>>
		                            ,pow_unit_t<unlib::      luminosity,std::ratio<2>>
		                            ,pow_unit_t<unlib::     temperature,std::ratio<4>>
		                            ,pow_unit_t<unlib::substance_amount,std::ratio<6>> >;

		using test_qa  = test_quantity::rescale_to<milli_scaling>;
		using test_qb  = quantity<test_unit_b, kilo_scaling, value_type>;

		using test_q_mul_ab = mul_unit_t<default_test_unit, test_unit_b>;
		CHECK( typeid(test_q_mul_ab) == typeid(test_unit_mab) );

		using test_q_div_ab = div_unit_t<default_test_unit, test_unit_b>;
		CHECK( typeid(test_q_div_ab) == typeid(test_unit_dab) );

		test_qa ua{42};
		test_qb ub{110};

		const auto a_mul_b = ua * ub;
		CHECK( typeid(a_mul_b) == typeid(quantity<test_q_mul_ab>) );
		CHECK( a_mul_b.get() == doctest::Approx(ua.get()*ub.get()) );

		const auto a_div_b = ua / ub;
		CHECK( typeid(a_div_b) == typeid(quantity<test_q_div_ab, std::ratio<1,1000000>>) );
		CHECK( a_div_b.get() == doctest::Approx(ua.get()/ub.get()) );
	}

	SUBCASE("math operations correctly calculate and scale and preserve tag ratio") {
		struct test_tag_id;
		using test_qa = test_quantity::retag<tag_t<test_tag_id>>;
		using test_qb = test_qa::rescale_to<kilo_scaling>;
		test_qa a{42};
		test_qb b{23};

		SUBCASE("scaling") {
			CHECK(typeid(decltype(a      )::scale_type) == typeid(  no_scaling) );
			CHECK(typeid(decltype(b      )::scale_type) == typeid(kilo_scaling) );
			CHECK(typeid(decltype(a*a    )::scale_type) == typeid(  no_scaling) );
			CHECK(typeid(decltype(a*b    )::scale_type) == typeid(kilo_scaling) );
			CHECK(typeid(decltype(b*b    )::scale_type) == typeid(mega_scaling) );
			CHECK(typeid(decltype(a*a*a  )::scale_type) == typeid(  no_scaling) );
			CHECK(typeid(decltype(a*a*b  )::scale_type) == typeid(kilo_scaling) );
			CHECK(typeid(decltype(b*b*b  )::scale_type) == typeid(giga_scaling) );
			CHECK(typeid(decltype(a*b*b  )::scale_type) == typeid(mega_scaling) );
			CHECK(typeid(decltype(a*a*a*a)::scale_type) == typeid(  no_scaling) );
			CHECK(typeid(decltype(a*a*a*b)::scale_type) == typeid(kilo_scaling) );
			CHECK(typeid(decltype(a*a*b*b)::scale_type) == typeid(mega_scaling) );
			CHECK(typeid(decltype(a*b*b*b)::scale_type) == typeid(giga_scaling) );
			CHECK(typeid(decltype(b*b*b*b)::scale_type) == typeid(tera_scaling) );

			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 3>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 2>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 1>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 0>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-1>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-2>>::scale_type) == typeid(no_scaling));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-3>>::scale_type) == typeid(no_scaling));

			CHECK(typeid(pow_quantity_t<test_qb,std::ratio< 3>>::scale_type) == typeid( giga_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio< 2>>::scale_type) == typeid( mega_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio< 1>>::scale_type) == typeid( kilo_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio< 0>>::scale_type) == typeid(   no_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio<-1>>::scale_type) == typeid( kilo_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio<-2>>::scale_type) == typeid( mega_scaling));
			CHECK(typeid(pow_quantity_t<test_qb,std::ratio<-3>>::scale_type) == typeid( giga_scaling));

			CHECK(typeid(sqrt_quantity_t<               test_qa               >::scale_type) == typeid(  no_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<test_qb,std::ratio<2>>>::scale_type) == typeid(kilo_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<test_qb,std::ratio<4>>>::scale_type) == typeid(mega_scaling));

			CHECK(typeid(sqrt_scale_t<scale_t<                  1,1000000000000000000>>) == typeid(scale_t<1         ,1000000000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,  10000000000000000>>) == typeid(scale_t<1         , 100000000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,    100000000000000>>) == typeid(scale_t<1         ,  10000000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,      1000000000000>>) == typeid(scale_t<1         ,   1000000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,        10000000000>>) == typeid(scale_t<1         ,    100000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,          100000000>>) == typeid(scale_t<1         ,     10000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,            1000000>>) == typeid(scale_t<1         ,      1000>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,              10000>>) == typeid(scale_t<1         ,       100>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,                100>>) == typeid(scale_t<1         ,        10>));
			CHECK(typeid(sqrt_scale_t<scale_t<                  1,1                  >>) == typeid(scale_t<1         ,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<                100,1                  >>) == typeid(scale_t<        10,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<              10000,1                  >>) == typeid(scale_t<       100,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<            1000000,1                  >>) == typeid(scale_t<      1000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<          100000000,1                  >>) == typeid(scale_t<     10000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<        10000000000,1                  >>) == typeid(scale_t<    100000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<      1000000000000,1                  >>) == typeid(scale_t<   1000000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<    100000000000000,1                  >>) == typeid(scale_t<  10000000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<  10000000000000000,1                  >>) == typeid(scale_t< 100000000,         1>));
			CHECK(typeid(sqrt_scale_t<scale_t<1000000000000000000,1                  >>) == typeid(scale_t<1000000000,         1>));

			CHECK(typeid(cbrt_scale_t<scale_t<                  1,1000000000000000000>>) == typeid(scale_t<      1,1000000>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,   1000000000000000>>) == typeid(scale_t<      1, 100000>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,      1000000000000>>) == typeid(scale_t<      1,  10000>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,         1000000000>>) == typeid(scale_t<      1,   1000>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,            1000000>>) == typeid(scale_t<      1,    100>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,               1000>>) == typeid(scale_t<      1,     10>));
			CHECK(typeid(cbrt_scale_t<scale_t<                  1,1                  >>) == typeid(scale_t<      1,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<               1000,1                  >>) == typeid(scale_t<     10,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<            1000000,1                  >>) == typeid(scale_t<    100,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<         1000000000,1                  >>) == typeid(scale_t<   1000,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<      1000000000000,1                  >>) == typeid(scale_t<  10000,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<   1000000000000000,1                  >>) == typeid(scale_t< 100000,      1>));
			CHECK(typeid(cbrt_scale_t<scale_t<1000000000000000000,1                  >>) == typeid(scale_t<1000000,      1>));

			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        nano<test_qa>,std::ratio<2>>>::scale_type) == typeid(  nano_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<       micro<test_qa>,std::ratio<2>>>::scale_type) == typeid( micro_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<       milli<test_qa>,std::ratio<2>>>::scale_type) == typeid( milli_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<       centi<test_qa>,std::ratio<2>>>::scale_type) == typeid( centi_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        deci<test_qa>,std::ratio<2>>>::scale_type) == typeid(  deci_scaling));
			CHECK(typeid(sqrt_quantity_t<                            test_qa                >::scale_type) == typeid(    no_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        deca<test_qa>,std::ratio<2>>>::scale_type) == typeid(  deca_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<       hecto<test_qa>,std::ratio<2>>>::scale_type) == typeid( hecto_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        kilo<test_qa>,std::ratio<2>>>::scale_type) == typeid(  kilo_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        mega<test_qa>,std::ratio<2>>>::scale_type) == typeid(  mega_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<        giga<test_qa>,std::ratio<2>>>::scale_type) == typeid(  giga_scaling));

			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<minute_scale<test_qa>,std::ratio<2>>>::scale_type) == typeid(minute_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<  hour_scale<test_qa>,std::ratio<2>>>::scale_type) == typeid(  hour_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<   day_scale<test_qa>,std::ratio<2>>>::scale_type) == typeid(   day_scaling));
			CHECK(typeid(sqrt_quantity_t<pow_quantity_t<  week_scale<test_qa>,std::ratio<2>>>::scale_type) == typeid(  week_scaling));

			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<       micro<test_qa>,std::ratio<3>>>::scale_type) == typeid( micro_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<       milli<test_qa>,std::ratio<3>>>::scale_type) == typeid( milli_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<       centi<test_qa>,std::ratio<3>>>::scale_type) == typeid( centi_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<        deci<test_qa>,std::ratio<3>>>::scale_type) == typeid(  deci_scaling));
			CHECK(typeid(cbrt_quantity_t<                            test_qa                >::scale_type) == typeid(    no_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<        deca<test_qa>,std::ratio<3>>>::scale_type) == typeid(  deca_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<       hecto<test_qa>,std::ratio<3>>>::scale_type) == typeid( hecto_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<        kilo<test_qa>,std::ratio<3>>>::scale_type) == typeid(  kilo_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<        mega<test_qa>,std::ratio<3>>>::scale_type) == typeid(  mega_scaling));

			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<minute_scale<test_qa>,std::ratio<3>>>::scale_type) == typeid(minute_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<  hour_scale<test_qa>,std::ratio<3>>>::scale_type) == typeid(  hour_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<   day_scale<test_qa>,std::ratio<3>>>::scale_type) == typeid(   day_scaling));
			CHECK(typeid(cbrt_quantity_t<pow_quantity_t<  week_scale<test_qa>,std::ratio<3>>>::scale_type) == typeid(  week_scaling));
		}

		SUBCASE("tags and tag ratios") {
			CHECK(typeid(decltype(a / b    )::tag_type) == typeid(no_tag                ) );
			CHECK(typeid(decltype(a * b    )::tag_type) == typeid(tag_t<test_tag_id,2,1>) );
			CHECK(typeid(decltype(a * b / b)::tag_type) == typeid(tag_t<test_tag_id,1,1>) );
			CHECK(typeid(decltype(a / b * a)::tag_type) == typeid(tag_t<test_tag_id,1,1>) );

			REQUIRE(typeid(                decltype(a*a    ) ::tag_type) == typeid(tag_t<test_tag_id,2,1>) );
			CHECK  (typeid(sqrt_quantity_t<decltype(a*a    )>::tag_type) == typeid(tag_t<test_tag_id,1,1>) );
			REQUIRE(typeid(                decltype(a*a*a*a) ::tag_type) == typeid(tag_t<test_tag_id,4,1>) );
			CHECK  (typeid(sqrt_quantity_t<decltype(a*a*a*a)>::tag_type) == typeid(tag_t<test_tag_id,2,1>) );

			REQUIRE(typeid(                decltype(a*b    ) ::tag_type) == typeid(tag_t<test_tag_id,2,1>) );
			REQUIRE(typeid(                decltype(a*b*a*b) ::tag_type) == typeid(tag_t<test_tag_id,4,1>) );
			CHECK  (typeid(sqrt_quantity_t<decltype(a*b*a*b)>::tag_type) == typeid(tag_t<test_tag_id,2,1>) );

			REQUIRE(typeid(                decltype(b*b    ) ::tag_type) == typeid(tag_t<test_tag_id,2,1>) );
			CHECK  (typeid(sqrt_quantity_t<decltype(b*b    )>::tag_type) == typeid(tag_t<test_tag_id,1,1>) );
			REQUIRE(typeid(                decltype(b*b*b*b) ::tag_type) == typeid(tag_t<test_tag_id,4,1>) );
			CHECK  (typeid(sqrt_quantity_t<decltype(b*b*b*b)>::tag_type) == typeid(tag_t<test_tag_id,2,1>) );
//
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 5>>::tag_type) == typeid(tag_t<test_tag_id, 5,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 4>>::tag_type) == typeid(tag_t<test_tag_id, 4,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 3>>::tag_type) == typeid(tag_t<test_tag_id, 3,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 2>>::tag_type) == typeid(tag_t<test_tag_id, 2,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 1>>::tag_type) == typeid(tag_t<test_tag_id, 1,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio< 0>>::tag_type) == typeid(        no_tag         ));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-1>>::tag_type) == typeid(tag_t<test_tag_id,-1,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-2>>::tag_type) == typeid(tag_t<test_tag_id,-2,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-3>>::tag_type) == typeid(tag_t<test_tag_id,-3,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-4>>::tag_type) == typeid(tag_t<test_tag_id,-4,1>));
			CHECK(typeid(pow_quantity_t<test_qa,std::ratio<-5>>::tag_type) == typeid(tag_t<test_tag_id,-5,1>));
		}
	}

	SUBCASE("integer quantities can be incremented and decremented") {
		using test_qi = test_quantity::revalue<int>;
		using test_qf = test_quantity;
		test_qi qi{42};
		test_qf qf{42.};

		CHECK(++qi == test_qi{43});
		CHECK(  qi == test_qi{43});
		CHECK(qi++ == test_qi{43});
		CHECK(qi   == test_qi{44});
		CHECK(--qi == test_qi{43});
		CHECK(  qi == test_qi{43});
		CHECK(qi-- == test_qi{43});
		CHECK(qi   == test_qi{42});

		//CHECK(++qf == test_qf{43.});
		//CHECK(  qf == test_qf{43.});
		//CHECK(qf++ == test_qf{43.});
		//CHECK(qf   == test_qf{44.});
		//CHECK(--qf == test_qf{43.});
		//CHECK(  qf == test_qf{43.});
		//CHECK(qf-- == test_qf{43.});
		//CHECK(qf   == test_qf{42.});
	}

}

SUBCASE("quantity casts") {
	using namespace unlib;
	using test_quantity  = quantity<default_test_unit, no_scaling, double>;

	SUBCASE("testing value_cast") {
		using test_qd  = test_quantity;
		using test_qi  = test_quantity::revalue<int>;

		test_qd qd{420000};

		//test_qi qi1{qd}; // doesn't compile
		const test_qi qi2{ value_cast<int>(qd) };
		CHECK(static_cast<int>(qd.get()) == qi2.get());

		const test_qi qi3{ value_cast(qd) };
		CHECK(static_cast<int>(qd.get()) == qi2.get());

		//qd = qi1; // doesn't compile
		qd = value_cast<double>(qi2);
		CHECK(qd.get() == doctest::Approx(qi2.get()));
		qd = value_cast(qi3);
		CHECK(qd.get() == doctest::Approx(qi3.get()));
	}

	SUBCASE("testing scale_cast") {
		using test_q   =      test_quantity;

		test_q q{420000};

		test_q qk1{q}; // scales can be converted implicitly
		to_kilo<test_quantity> qk2{ scale_cast<kilo_scaling>(q)};
		CHECK(q.get() == doctest::Approx(qk2.get()*1000));
		to_kilo<test_quantity> qk3{ scale_cast(q)};
		CHECK(q.get() == doctest::Approx(qk3.get()*1000));

		q = qk2; // scales can be converted
		q = scale_cast<no_scaling>(qk2);
		CHECK(q.get() == doctest::Approx(qk2.get()*1000));
		q = scale_cast(qk3);
		CHECK(q.get() == doctest::Approx(qk3.get()*1000));
	}

	SUBCASE("testing tag_cast") {
		using namespace unlib;
		using test_qa  = test_quantity::retag<tag_t<struct tag_a>>;
		using test_qb  = test_quantity::retag<tag_t<struct tag_b>>;

		test_qa qa{420000};

		//test_qb qb1{qa}; // doesn't compile
		test_qb qb2{ tag_cast<tag_t<struct tag_b>>(qa)};
		CHECK(qa.get() == doctest::Approx(qb2.get()));
		test_qb qb3{ tag_cast(qa)};
		CHECK(qa.get() == doctest::Approx(qb3.get()));

		//qa = qb2; // doesn't compile
		qa = tag_cast<tag_t<struct tag_a>>(qb2);
		CHECK(qa.get() == doctest::Approx(qb2.get()));
		qa = tag_cast(qb3);
		CHECK(qa.get() == doctest::Approx(qb3.get()));
	}

	SUBCASE("testing quantity_cast") {
		using test_qdka  = quantity<default_test_unit,  kilo_scaling, double, tag_t<struct tag_a>>;
		using test_qika  = quantity<default_test_unit,  kilo_scaling, int   , tag_t<struct tag_a>>;
		using test_qdma  = quantity<default_test_unit, milli_scaling, double, tag_t<struct tag_a>>;
		using test_qdkb  = quantity<default_test_unit,  kilo_scaling, double, tag_t<struct tag_b>>;
		using test_qimb  = quantity<default_test_unit, milli_scaling, double, tag_t<struct tag_b>>;

		test_qdka qdka{42};

		//test_qika qika1{qdka}; // doesn't compile
		test_qdma qdma1{qdka}; // scales can be converted implicitly
		//test_qdkb qdkb1{qdka}; // doesn't compile
		//test_qimb qimb1{qdka}; // doesn't compile

		test_qika qika2{ quantity_cast<test_qika>(qdka)}; CHECK(qika2.get() == doctest::Approx(qdka.get()          ));
		test_qdma qdma2{ quantity_cast<test_qdma>(qdka)}; CHECK(qdma2.get() == doctest::Approx(qdka.get()*1000*1000));
		test_qdkb qdkb2{ quantity_cast<test_qdkb>(qdka)}; CHECK(qdkb2.get() == doctest::Approx(qdka.get()          ));
		test_qimb qimb2{ quantity_cast<test_qimb>(qdka)}; CHECK(qimb2.get() == doctest::Approx(qdka.get()*1000*1000));

		test_qika qika3{ quantity_cast(qdka)}; CHECK(qika3.get() == doctest::Approx(qdka.get()          ));
		test_qdma qdma3{ quantity_cast(qdka)}; CHECK(qdma3.get() == doctest::Approx(qdka.get()*1000*1000));
		test_qdkb qdkb3{ quantity_cast(qdka)}; CHECK(qdkb3.get() == doctest::Approx(qdka.get()          ));
		test_qimb qimb3{ quantity_cast(qdka)}; CHECK(qimb3.get() == doctest::Approx(qdka.get()*1000*1000));
	}
}

}
