#include <unlib/quantity.hpp>
#include <string>
#include <functional>

#define CATCH_CONFIG_PREFIX_ALL
#include <catch2/catch.hpp>

#include <unlib/test/unlib_test.hpp>

namespace {

using default_test_unit = unlib::create_unit_t<unlib::pow_unit_t<unlib::detail::            time,1>
                                              ,unlib::pow_unit_t<unlib::detail::            mass,2>
                                              ,unlib::pow_unit_t<unlib::detail::          length,3>
                                              ,unlib::pow_unit_t<unlib::detail::         current,4>
                                              ,unlib::pow_unit_t<unlib::detail::      luminosity,5>
                                              ,unlib::pow_unit_t<unlib::detail::     temperature,6>
                                              ,unlib::pow_unit_t<unlib::detail::substance_amount,7> >;

}

CATCH_TEST_CASE("quantities type operations", "[quantity]") {
	using namespace unlib;
	using scale_type = scale<1,2>;
	using value_type = double;
	using test_quantity = quantity<default_test_unit, scale_type, value_type>;

	CATCH_SECTION("template parameters properly propagate to quantity") {
		CATCH_CHECK( typeid(default_test_unit) == typeid(test_quantity::unit_type)  );
		CATCH_CHECK( typeid(value_type)        == typeid(test_quantity::value_type) );

		CATCH_CHECK( typeid(scale_type)        == typeid(test_quantity::scale_type) );
		CATCH_CHECK( typeid(void)              == typeid(test_quantity::tag_type)   );

		CATCH_CHECK( typeid(exponent<1>)       == typeid(test_quantity::            time_exponent) );
		CATCH_CHECK( typeid(exponent<2>)       == typeid(test_quantity::            mass_exponent) );
		CATCH_CHECK( typeid(exponent<3>)       == typeid(test_quantity::          length_exponent) );
		CATCH_CHECK( typeid(exponent<4>)       == typeid(test_quantity::         current_exponent) );
		CATCH_CHECK( typeid(exponent<5>)       == typeid(test_quantity::      luminosity_exponent) );
		CATCH_CHECK( typeid(exponent<6>)       == typeid(test_quantity::     temperature_exponent) );
		CATCH_CHECK( typeid(exponent<7>)       == typeid(test_quantity::substance_amount_exponent) );
	}

	CATCH_SECTION("quantities can be retagged") {
		struct foo_tag {};
		struct bar_tag {};
		using test_q_foo = test_quantity::retag<foo_tag>;
		using test_q_bar = test_q_foo::retag<bar_tag>;

		CATCH_REQUIRE( typeid(test_quantity::tag_type) == typeid(void   ) );
		CATCH_CHECK  ( typeid(test_q_foo::tag_type)    == typeid(foo_tag) );
		CATCH_CHECK  ( typeid(test_q_bar::tag_type)    == typeid(bar_tag) );
	}

	CATCH_SECTION("quantities can be rescaled") {
		using test_quantity_a = test_quantity;
		CATCH_REQUIRE( typeid(test_quantity_a::scale_type)                           == typeid(scale< 1, 2>) );
		CATCH_CHECK  ( typeid(test_quantity_a::rescale_to<scale<47,11>>::scale_type) == typeid(scale<47,11>) );
		CATCH_CHECK  ( typeid(test_quantity_a::rescale_by<scale< 4, 1>>::scale_type) == typeid(scale< 2, 1>) );

		using test_quantity_b = unlib::quantity<default_test_unit>;
		CATCH_CHECK( typeid( to_atto <test_quantity_b> ) == typeid( quantity<default_test_unit,  atto_scaling> ) );
		CATCH_CHECK( typeid( to_femto<test_quantity_b> ) == typeid( quantity<default_test_unit, femto_scaling> ) );
		CATCH_CHECK( typeid( to_pico <test_quantity_b> ) == typeid( quantity<default_test_unit,  pico_scaling> ) );
		CATCH_CHECK( typeid( to_nano <test_quantity_b> ) == typeid( quantity<default_test_unit,  nano_scaling> ) );
		CATCH_CHECK( typeid( to_micro<test_quantity_b> ) == typeid( quantity<default_test_unit, micro_scaling> ) );
		CATCH_CHECK( typeid( to_milli<test_quantity_b> ) == typeid( quantity<default_test_unit, milli_scaling> ) );
		CATCH_CHECK( typeid( to_centi<test_quantity_b> ) == typeid( quantity<default_test_unit, centi_scaling> ) );
		CATCH_CHECK( typeid( to_deci <test_quantity_b> ) == typeid( quantity<default_test_unit,  deci_scaling> ) );
		CATCH_CHECK( typeid( to_deca <test_quantity_b> ) == typeid( quantity<default_test_unit,  deca_scaling> ) );
		CATCH_CHECK( typeid( to_hecto<test_quantity_b> ) == typeid( quantity<default_test_unit, hecto_scaling> ) );
		CATCH_CHECK( typeid( to_kilo <test_quantity_b> ) == typeid( quantity<default_test_unit,  kilo_scaling> ) );
		CATCH_CHECK( typeid( to_mega <test_quantity_b> ) == typeid( quantity<default_test_unit,  mega_scaling> ) );
		CATCH_CHECK( typeid( to_giga <test_quantity_b> ) == typeid( quantity<default_test_unit,  giga_scaling> ) );
		CATCH_CHECK( typeid( to_tera <test_quantity_b> ) == typeid( quantity<default_test_unit,  tera_scaling> ) );
		CATCH_CHECK( typeid( to_peta <test_quantity_b> ) == typeid( quantity<default_test_unit,  peta_scaling> ) );
		CATCH_CHECK( typeid( to_exa  <test_quantity_b> ) == typeid( quantity<default_test_unit,   exa_scaling> ) );

		CATCH_CHECK( typeid( to_milli<to_kilo<test_quantity_b>> ) == typeid( to_milli<test_quantity_b> ) );
	}
}

CATCH_TEST_CASE("quantity values", "[quantity]") {
	using namespace unlib;

	using value_type = double;
	using test_quantity = quantity<default_test_unit, no_scaling, value_type>;

	CATCH_SECTION("quantity is default-initialized") {
		{
			test_quantity q{1000}; // make stack memory dirty
		}
		{
			test_quantity q;
			CATCH_CHECK(q.is_near_zero());
		}
	}
	CATCH_SECTION("value is stored correctly") {
		const value_type v = 42.;

		const test_quantity q1{v};
		CATCH_CHECK( v == q1.get() );
		const test_quantity q2(q1);
		CATCH_CHECK( v == q2.get() );
		test_quantity q3;
		CATCH_CHECK( q3.is_near_zero() );
		q3 = q1;
		CATCH_CHECK( not q3.is_near_zero() );
		CATCH_CHECK( v == q3.get() );
	}

	CATCH_SECTION("is_near_zero") {
		using test_q = test_quantity::rescale_to<scale<1,2>>;

		const value_type epsilon = 150*std::numeric_limits<value_type>::epsilon();

		const test_q q1{};
		CATCH_CHECK( q1.is_near_zero() );
		const test_q q2{epsilon};
		CATCH_CHECK( not q2.is_near_zero() );
	}

	CATCH_SECTION("values are rescaled properly") {
		const int original_value = 42000;
		int downscaled_value = detail::rescale_value<milli_scaling, no_scaling>(original_value);
		CATCH_CHECK( downscaled_value == 1000*original_value );

		int upscaled_value = detail::rescale_value< kilo_scaling, no_scaling >(original_value);
		CATCH_CHECK( upscaled_value == original_value/1000 );

		struct test_value_type {
			bool multiplication_called{false};

			test_value_type operator*(test_value_type)                        {multiplication_called=true; return *this;}
		};

		test_value_type test_value;
		detail::rescale_value<    no_scaling,    no_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  deci_scaling,  deci_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< centi_scaling, centi_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< milli_scaling, milli_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value< micro_scaling, micro_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  kilo_scaling,  kilo_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
		detail::rescale_value<  mega_scaling,  mega_scaling >(test_value); CATCH_CHECK(not test_value.multiplication_called); test_value.multiplication_called=false;
	}

	CATCH_SECTION("values can be re-scaled implicitly through constructors") {
		const value_type v = 42;

		         test_quantity   q1{  v*1000000000000000000}; CATCH_CHECK( v*1000000000000000000 ==  q1.get() );
		to_deca <test_quantity> daq{ q1                    }; CATCH_CHECK( v*100000000000000000  == daq.get() );
		to_hecto<test_quantity>  hq{daq                    }; CATCH_CHECK( v*10000000000000000   ==  hq.get() );
		to_kilo <test_quantity>  kq{ hq                    }; CATCH_CHECK( v*1000000000000000    ==  kq.get() );
		to_mega <test_quantity>  Mq{ kq                    }; CATCH_CHECK( v*1000000000000       ==  Mq.get() );
		to_giga <test_quantity>  Gq{ Mq                    }; CATCH_CHECK( v*1000000000          ==  Gq.get() );

//		to_exa  <test_quantity>  Eq{ v }; CATCH_CHECK( v                     ==  Eq.get() );
//		to_peta <test_quantity>  Pq{ Eq}; CATCH_CHECK( v*1000                ==  Pq.get() );
//		to_tera <test_quantity>  Tq{ Pq}; CATCH_CHECK( v*1000000             ==  Tq.get() );
//		to_giga <test_quantity>  Gq{ Tq}; CATCH_CHECK( v*1000000000          ==  Gq.get() );
//		to_mega <test_quantity>  Mq{ Gq}; CATCH_CHECK( v*1000000000000       ==  Mq.get() );
//		to_kilo <test_quantity>  kq{ Mq}; CATCH_CHECK( v*1000000000000000    ==  kq.get() );
//		to_hecto<test_quantity>  hq{ kq}; CATCH_CHECK( v*10000000000000000   ==  hq.get() );
//		to_deca <test_quantity> daq{ hq}; CATCH_CHECK( v*100000000000000000  == daq.get() );
//		         test_quantity   q1{daq}; CATCH_CHECK( v*1000000000000000000 ==  q1.get() );
		         test_quantity   q2{ v }; CATCH_CHECK( v                     ==  q2.get() );
		to_deci <test_quantity>  dq{ q2}; CATCH_CHECK( v*10                  ==  dq.get() );
		to_centi<test_quantity>  cq{ dq}; CATCH_CHECK( v*100                 ==  cq.get() );
		to_milli<test_quantity>  mq{ dq}; CATCH_CHECK( v*1000                ==  mq.get() );
		to_micro<test_quantity>  uq{ mq}; CATCH_CHECK( v*1000000             ==  uq.get() );
		to_nano <test_quantity>  nq{ uq}; CATCH_CHECK( v*1000000000          ==  nq.get() );
//		to_pico <test_quantity>  pq{ nq}; CATCH_CHECK( v*1000000000000       ==  pq.get() );
//		to_femto<test_quantity>  fq{ pq}; CATCH_CHECK( v*1000000000000000    ==  fq.get() );
//		to_atto <test_quantity>  aq{ fq}; CATCH_CHECK( v*1000000000000000000 ==  aq.get() );
	}

	CATCH_SECTION("values can be re-scaled through get") {
		const value_type v = 42000;

		test_quantity q1{v};
		CATCH_CHECK( v*1000  == q1.get_scaled<milli_scaling>() );

		test_quantity q2{v};
		CATCH_CHECK( v/1000. == q2.get_scaled<kilo_scaling>() );
	}

	CATCH_SECTION("quantities can be compared") {
		using test_mq = test_quantity::rescale_to<milli_scaling>;

		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{v};
		test_quantity q3{v*2};
		test_mq mq1{q1};

		CATCH_CHECK( q1 == q1 );
		CATCH_CHECK( q1 == q2 );
		CATCH_CHECK( q2 == q1 );
		CATCH_CHECK( q2 == q2 );

		CATCH_CHECK( q1 <= q2 );
		CATCH_CHECK( q1 >= q2 );
		CATCH_CHECK( q2 <= q1 );
		CATCH_CHECK( q2 >= q1 );

		CATCH_CHECK( q1 != q3 );
		CATCH_CHECK( q3 != q1 );

		CATCH_CHECK( q1 <  q3 );
		CATCH_CHECK( q3 >  q1 );
		CATCH_CHECK( q1 <= q3 );
		CATCH_CHECK( q3 >= q1 );

		CATCH_CHECK( mq1 ==  q1 );
		CATCH_CHECK(  q1 == mq1 );
		CATCH_CHECK( mq1 ==  q2 );
		CATCH_CHECK(  q2 == mq1 );

		CATCH_CHECK( mq1 <=  q2 );
		CATCH_CHECK( mq1 >=  q2 );
		CATCH_CHECK(  q2 <= mq1 );
		CATCH_CHECK(  q2 >= mq1 );

		CATCH_CHECK( mq1 !=  q3 );
		CATCH_CHECK(  q3 != mq1 );

		CATCH_CHECK( mq1 <   q3 );
		CATCH_CHECK(  q3 >  mq1 );
		CATCH_CHECK( mq1 <=  q3 );
		CATCH_CHECK(  q3 >= mq1 );
	}

	CATCH_SECTION("is_near") {
			using value_type = double;
			using test_q = test_quantity::rescale_to<scale<1,2>>;

			const value_type epsilon = std::numeric_limits<value_type>::epsilon();

			const test_q q0{}, q1{epsilon}, q2{42}, q3{42}, q4{42 + epsilon}, q5{2 * 42.};

			CATCH_CHECK(     q0.is_near(q1) );
			CATCH_CHECK( not q0.is_near(q2) );
			CATCH_CHECK( not q0.is_near(q3) );
			CATCH_CHECK( not q0.is_near(q4) );
			CATCH_CHECK( not q0.is_near(q5) );

			CATCH_CHECK( not q1.is_near(q2) );
			CATCH_CHECK( not q1.is_near(q3) );
			CATCH_CHECK( not q1.is_near(q3) );
			CATCH_CHECK( not q1.is_near(q4) );
			CATCH_CHECK( not q1.is_near(q5) );

			CATCH_CHECK(     q2.is_near(q3) );
			CATCH_CHECK(     q2.is_near(q4) );
			CATCH_CHECK( not q2.is_near(q5) );

			CATCH_CHECK(     q3.is_near(q4) );
			CATCH_CHECK( not q3.is_near(q5) );

			CATCH_CHECK( not q4.is_near(q5) );
		}

	CATCH_SECTION("quantities can be signed") {
		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{-v};

		CATCH_CHECK(+q1 == q1);
		CATCH_CHECK(-q1 == q2);
		CATCH_CHECK(-q2 == q1);
	}
}

CATCH_TEST_CASE("quantities can be added and subtracted", "[quantity]") {
	using namespace unlib;

	using value_type = double;
	using test_quantity  = quantity<default_test_unit,    no_scaling, value_type>;

	CATCH_SECTION("quantities can be added and subtracted") {
		using test_mq = test_quantity::rescale_to<milli_scaling>;

		const value_type v = 42;

		test_quantity q1{v};
		test_quantity q2{v*2};
		test_quantity q3{v};
		q1 += q2;
		q3 += q1;

		CATCH_CHECK( q1.get() == 3*v );
		CATCH_CHECK( q2.get() == 2*v );
		CATCH_CHECK( q3.get() == 4*v );

		CATCH_CHECK( (q1+q2).get() == 5*v );

		test_mq mqu1{q1};
		mqu1 += q1;
		CATCH_CHECK( mqu1.get() == 6000*v );

		q1 += mqu1;
		CATCH_CHECK( q1.get() == 9*v );


		q1 -= mqu1;
		CATCH_CHECK( q1.get() == 3*v );

		q1 -= q2;
		CATCH_CHECK( q1.get() == v );

		CATCH_CHECK( (q2-q1).get() == v );
	}

	CATCH_SECTION("quantities can be multiplied and divided") {
		using test_unit_b   = create_unit_t<pow_unit_t<unlib::detail::            time,7>
		                                   ,pow_unit_t<unlib::detail::            mass,6>
		                                   ,pow_unit_t<unlib::detail::          length,5>
		                                   ,pow_unit_t<unlib::detail::         current,4>
		                                   ,pow_unit_t<unlib::detail::      luminosity,3>
		                                   ,pow_unit_t<unlib::detail::     temperature,2>
		                                   ,pow_unit_t<unlib::detail::substance_amount,1> >;
		using test_unit_mab = create_unit_t<pow_unit_t<unlib::detail::            time,8>
		                                   ,pow_unit_t<unlib::detail::            mass,8>
		                                   ,pow_unit_t<unlib::detail::          length,8>
		                                   ,pow_unit_t<unlib::detail::         current,8>
		                                   ,pow_unit_t<unlib::detail::      luminosity,8>
		                                   ,pow_unit_t<unlib::detail::     temperature,8>
		                                   ,pow_unit_t<unlib::detail::substance_amount,8> >;
		using test_unit_dab = create_unit_t<pow_unit_t<unlib::detail::            time,-6>
		                                   ,pow_unit_t<unlib::detail::            mass,-4>
		                                   ,pow_unit_t<unlib::detail::          length,-2>
		                                   ,pow_unit_t<unlib::detail::         current, 0>
		                                   ,pow_unit_t<unlib::detail::      luminosity, 2>
		                                   ,pow_unit_t<unlib::detail::     temperature, 4>
		                                   ,pow_unit_t<unlib::detail::substance_amount, 6> >;

		using test_qa  = test_quantity::rescale_to<milli_scaling>;
		using test_qb  = quantity<test_unit_b, kilo_scaling, value_type>;

		using test_q_mul_ab = mul_unit_t<default_test_unit, test_unit_b>;
		CATCH_CHECK( typeid(test_q_mul_ab) == typeid(test_unit_mab) );

		using test_q_div_ab = div_unit_t<default_test_unit, test_unit_b>;
		CATCH_CHECK( typeid(test_q_div_ab) == typeid(test_unit_dab) );

		test_qa ua{42};
		test_qb ub{110};

		const auto a_mul_b = ua * ub;
		CATCH_CHECK( test::demangle(a_mul_b) == test::demangle<quantity<test_q_mul_ab>>() );
		CATCH_CHECK( a_mul_b.get() == ua.get()*ub.get() );

		const auto a_div_b = ua / ub;
		CATCH_CHECK( test::demangle(a_div_b) == test::demangle<quantity<test_q_div_ab, std::ratio<1,1000000>>>() );
		CATCH_CHECK( a_div_b.get() == ua.get()/ub.get() );
	}

}

CATCH_TEST_CASE("quantity casts", "[unit]") {
	using namespace unlib;
	using test_quantity  = quantity<default_test_unit, no_scaling, double>;

	CATCH_SECTION("testing value_cast", "[unit]") {
		using test_qd  = test_quantity;
		using test_qi  = test_quantity::revalue<int>;

		test_qd qd{420000};

		//test_qi qi1{qd}; // doesn't compile
		const test_qi qi2{ value_cast<int>(qd) };
		CATCH_CHECK(static_cast<int>(qd.get()) == qi2.get());

		const test_qi qi3{ value_cast(qd) };
		CATCH_CHECK(static_cast<int>(qd.get()) == qi2.get());

		//qd = qi1; // doesn't compile
		qd = value_cast<double>(qi2);
		CATCH_CHECK(qd.get() == static_cast<double>(qi2.get()));
		qd = value_cast(qi3);
		CATCH_CHECK(qd.get() == static_cast<double>(qi3.get()));
	}

	CATCH_SECTION("testing scale_cast", "[unit]") {
		using test_q   =      test_quantity;

		test_q q{420000};

		test_q qk1{q}; // scales can be converted implicitly
		to_kilo<test_quantity> qk2{ scale_cast<kilo_scaling>(q)};
		CATCH_CHECK(q.get() == qk2.get()*1000);
		to_kilo<test_quantity> qk3{ scale_cast(q)};
		CATCH_CHECK(q.get() == qk3.get()*1000);

		q = qk2; // scales can be converted
		q = scale_cast<no_scaling>(qk2);
		CATCH_CHECK(q.get() == qk2.get()*1000);
		q = scale_cast(qk3);
		CATCH_CHECK(q.get() == qk3.get()*1000);
	}

	CATCH_SECTION("testing tag_cast", "[unit]") {
		using namespace unlib;
		using test_qa  = test_quantity::retag<struct tag_a>;
		using test_qb  = test_quantity::retag<struct tag_b>;

		test_qa qa{420000};

		//test_qb qb1{qa}; // doesn't compile
		test_qb qb2{ tag_cast<struct tag_b>(qa)};
		CATCH_CHECK(qa.get() == qb2.get());
		test_qb qb3{ tag_cast(qa)};
		CATCH_CHECK(qa.get() == qb3.get());

		//qa = qb2; // doesn't compile
		qa = tag_cast<struct tag_a>(qb2);
		CATCH_CHECK(qa.get() == qb2.get());
		qa = tag_cast(qb3);
		CATCH_CHECK(qa.get() == qb3.get());
	}

	CATCH_SECTION("testing quantity_cast", "[unit]") {
		using test_qdka  = quantity<default_test_unit,  kilo_scaling, double, struct tag_a>;
		using test_qika  = quantity<default_test_unit,  kilo_scaling, int   , struct tag_a>;
		using test_qdma  = quantity<default_test_unit, milli_scaling, double, struct tag_a>;
		using test_qdkb  = quantity<default_test_unit,  kilo_scaling, double, struct tag_b>;
		using test_qimb  = quantity<default_test_unit, milli_scaling, double, struct tag_b>;

		test_qdka qdka{42};

		//test_qika qika1{qdka}; // doesn't compile
		test_qdma qdma1{qdka}; // scales can be converted implicitly
		//test_qdkb qdkb1{qdka}; // doesn't compile
		//test_qimb qimb1{qdka}; // doesn't compile

		test_qika qika2{ quantity_cast<test_qika>(qdka)}; CATCH_CHECK(qika2.get() == qdka.get());
		test_qdma qdma2{ quantity_cast<test_qdma>(qdka)}; CATCH_CHECK(qdma2.get() == qdka.get()*1000*1000);
		test_qdkb qdkb2{ quantity_cast<test_qdkb>(qdka)}; CATCH_CHECK(qdkb2.get() == qdka.get());
		test_qimb qimb2{ quantity_cast<test_qimb>(qdka)}; CATCH_CHECK(qimb2.get() == qdka.get()*1000*1000);

		test_qika qika3{ quantity_cast(qdka)}; CATCH_CHECK(qika3.get() == qdka.get());
		test_qdma qdma3{ quantity_cast(qdka)}; CATCH_CHECK(qdma3.get() == qdka.get()*1000*1000);
		test_qdkb qdkb3{ quantity_cast(qdka)}; CATCH_CHECK(qdkb3.get() == qdka.get());
		test_qimb qimb3{ quantity_cast(qdka)}; CATCH_CHECK(qimb3.get() == qdka.get()*1000*1000);
	}
}
