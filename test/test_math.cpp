#include <unlib/math.hpp>

#include <doctest/doctest.h>

#include <unlib/common.hpp>
#include <unlib/test/unlib_test.hpp>

namespace {

struct comparison_table_result {
	bool                        value;
	bool                        nominal;
	bool                        fraction;
	bool                        nominal_fraction;

	int                         line;
};

template<typename Float>
struct zero_comparison_table_entry : comparison_table_result {
	Float                      val;
	Float                      tol1;
	Float                      tol2;

	zero_comparison_table_entry(comparison_table_result ctr, Float v, Float t1, Float t2)
	: comparison_table_result{ctr}, val{v}, tol1{t1}, tol2{t2} {}
};

template<typename Float>
struct comparison_table_entry : comparison_table_result {
	Float                      lhs;
	Float                      rhs;
	Float                      tol1;
	Float                      tol2;

	comparison_table_entry(comparison_table_result ctr, Float l, Float r, Float t1, Float t2)
	: comparison_table_result{ctr}, lhs{l}, rhs{r}, tol1{t1}, tol2{t2} {}
};

template<typename Float>
struct comparison_tests {
	using Watt = unlib::watt<Float>;
	using kW = unlib::kilo<Watt>;
	using zero_entry_t = zero_comparison_table_entry<Float>;
	using      entry_t =      comparison_table_entry<Float>;

	static void is_near_v(const entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.value << " == comparison_tests<" << typeid(Float).name() << ">::is_near_v(" << entry.lhs << ", " << entry.rhs << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( entry.value == unlib::is_near(        entry.lhs  ,        entry.rhs  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( entry.value == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( entry.value == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( entry.value == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( entry.value == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_near_v(const zero_entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.value << " == comparison_tests<" << typeid(Float).name() << ">::is_near_zero_v(" << entry.val << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( entry.value == unlib::is_near_zero(        entry.val  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( entry.value == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( entry.value == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( entry.value == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_near_n(const entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.nominal << " == comparison_tests<" << typeid(Float).name() << ">::is_near_n(" << entry.lhs << ", " << entry.rhs << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( entry.nominal == unlib::is_near(        entry.lhs  ,        entry.rhs  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( entry.nominal == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( entry.nominal == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( entry.nominal == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( entry.nominal == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_near_n(const zero_entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.nominal << " == comparison_tests<" << typeid(Float).name() << ">::is_near_zero_n(" << entry.val << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( entry.nominal == unlib::is_near_zero(        entry.val  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( entry.nominal == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( entry.nominal == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( entry.nominal == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_near_f(const entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.fraction << " == comparison_tests<" << typeid(Float).name() << ">::is_near_f(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( entry.fraction == unlib::is_near(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_near_f(const zero_entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.fraction << " == comparison_tests<" << typeid(Float).name() << ">::is_near_zero_f(" << entry.val << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( entry.fraction == unlib::is_near_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( entry.fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( entry.fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_near_fn(const entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.nominal_fraction << " == comparison_tests<" << typeid(Float).name() << ">::is_near_fn(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( entry.nominal_fraction == unlib::is_near(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	static void is_near_fn(const zero_entry_t& entry) {
		INFO(__FILE__ << ":" << entry.line << ": " << entry.nominal_fraction << " == comparison_tests<" << typeid(Float).name() << ">::is_near_zero_fn(" << entry.val << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( entry.nominal_fraction == unlib::is_near_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( entry.nominal_fraction == unlib::is_near_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	template<typename TEntry>
	static void is_near(const TEntry& entry) {
		is_near_v(entry);
		is_near_n(entry);
		is_near_f(entry);
		is_near_fn(entry);
	}

	static void is_smaller_v(const entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.lhs, entry.rhs, entry.value);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_v(" << entry.lhs << ", " << entry.rhs << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller(        entry.lhs  ,        entry.rhs  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_smaller_v(const zero_entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.val, Float{}, entry.value);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_zero_v(" << entry.val << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller_zero(        entry.val  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_smaller_n(const entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.lhs, entry.rhs, entry.nominal);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_n(" << entry.lhs << ", " << entry.rhs << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller(        entry.lhs  ,        entry.rhs  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_smaller_n(const zero_entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.val, Float{}, entry.nominal);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_zero_n(" << entry.val << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller_zero(        entry.val  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_smaller_f(const entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.lhs, entry.rhs, entry.fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_f(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_smaller_f(const zero_entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.val, Float{}, entry.fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_zero_f(" << entry.val << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( smaller == unlib::is_smaller_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_smaller_fn(const entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.lhs, entry.rhs, entry.nominal_fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_fn(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( smaller == unlib::is_smaller(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	static void is_smaller_fn(const zero_entry_t& entry) {
		const bool smaller = is_smaller_helper(entry.val, Float{}, entry.nominal_fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << smaller << " == comparison_tests<" << typeid(Float).name() << ">::is_smaller_zero_fn(" << entry.val << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( smaller == unlib::is_smaller_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( smaller == unlib::is_smaller_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	template<typename TEntry>
	static void is_smaller(const TEntry& entry) {
		is_smaller_v(entry);
		is_smaller_n(entry);
		is_smaller_f(entry);
		is_smaller_fn(entry);
	}

	static void is_greater_v(const entry_t& entry) {
		const bool greater = is_greater_helper(entry.lhs, entry.rhs, entry.value);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_v(" << entry.lhs << ", " << entry.rhs << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater(        entry.lhs  ,        entry.rhs  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_greater_v(const zero_entry_t& entry) {
		const bool greater = is_greater_helper(entry.val, Float{}, entry.value);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_zero_v(" << entry.val << ", tolerance_value(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater_zero(        entry.val  , unlib::tolerance_value(        entry.tol1  )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_value(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_value(kW{Watt{entry.tol1}})) );
	}
	static void is_greater_n(const entry_t& entry) {
		const bool greater = is_greater_helper(entry.lhs, entry.rhs, entry.nominal);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_n(" << entry.lhs << ", " << entry.rhs << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater(        entry.lhs  ,        entry.rhs  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_greater_n(const zero_entry_t& entry) {
		const bool greater = is_greater_helper(entry.val, Float{}, entry.nominal);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_zero_n(" << entry.val << ", tolerance_nominal(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater_zero(        entry.val  , unlib::tolerance_nominal(        entry.tol1  )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(   Watt{entry.tol1} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_nominal(kW{Watt{entry.tol1}})) );
	}
	static void is_greater_f(const entry_t& entry) {
		const bool greater = is_greater_helper(entry.lhs, entry.rhs, entry.fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_f(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_greater_f(const zero_entry_t& entry) {
		const bool greater = is_greater_helper(entry.val, Float{}, entry.fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_zero_f(" << entry.val << ", tolerance_fraction(" << entry.tol1 << "))");
		CHECK( greater == unlib::is_greater_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1/1000))) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1     ))) );
	}
	static void is_greater_fn(const entry_t& entry) {
		const bool greater = is_greater_helper(entry.lhs, entry.rhs, entry.nominal_fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_fn(" << entry.lhs << ", " << entry.rhs << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( greater == unlib::is_greater(        entry.lhs  ,        entry.rhs  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(   Watt{entry.lhs} ,kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},   Watt{entry.rhs} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater(kW{Watt{entry.lhs}},kW{Watt{entry.rhs}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	static void is_greater_fn(const zero_entry_t& entry) {
		const bool greater = is_greater_helper(entry.val, Float{}, entry.nominal_fraction);
		INFO(__FILE__ << ":" << entry.line << ": " << greater << " == comparison_tests<" << typeid(Float).name() << ">::is_greater_zero_fn(" << entry.val << ", tolerance_fraction(" << entry.tol1 << ")" << ", tolerance_nominal(" << entry.tol2 << "))");
		CHECK( greater == unlib::is_greater_zero(        entry.val  , unlib::tolerance_fraction<Float>(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(        entry.tol2  )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(   Watt{entry.val} , unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<kW   >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(   Watt{entry.tol2} )) );
		CHECK( greater == unlib::is_greater_zero(kW{Watt{entry.val}}, unlib::tolerance_fraction<Watt >(unlib::fraction<Float>(entry.tol1)), unlib::tolerance_nominal(kW{Watt{entry.tol2}})) );
	}
	template<typename TEntry>
	static void is_greater(const TEntry& entry) {
		is_greater_v(entry);
		is_greater_n(entry);
		is_greater_f(entry);
		is_greater_fn(entry);
	}

private:
	static bool is_smaller_helper(Float lhs, Float rhs, bool is_near) {return !is_near && lhs < rhs;}
	static bool is_greater_helper(Float lhs, Float rhs, bool is_near) {return !is_near && lhs > rhs;}
};

}

TEST_CASE("math") {
//	CHECK( unlib::is_smaller(1.0, 0.99, unlib::tolerance_value(0.1)) );
//	CHECK( unlib::is_smaller(0.99, 1.0, unlib::tolerance_value(0.1)) );

	using namespace unlib;

	using value_type = double;
	using unit_a = unit<exponent_t<1>
	                   ,exponent_t<2>
	                   ,exponent_t<3>
	                   ,exponent_t<4>
	                   ,exponent_t<5>
	                   ,exponent_t<6>
	                   ,exponent_t<7>>;
	using scale_type = scale_t<100,1>;
	using quantity_a = quantity<unit_a, scale_type, value_type>;

	SUBCASE("abs") {
		quantity_a gt{ 42};
		quantity_a z {  0};
		quantity_a lt{-gt.get()};

		CHECK(unlib::abs(gt) == gt);
		CHECK(unlib::abs( z) ==  z);
		CHECK(unlib::abs(lt) == gt);
	}

	SUBCASE("pow") {
		using quantity0 = quantity<dimensionless, scale_t<1>, value_type>;
		using quantity1 = quantity<unit_a, scale_t<1000000,1>, value_type>;
		using quantity2 = quantity< unit< exponent_t<1+1>
		                                , exponent_t<2+2>
		                                , exponent_t<3+3>
		                                , exponent_t<4+4>
		                                , exponent_t<5+5>
		                                , exponent_t<6+6>
		                                , exponent_t<7+7>>
		                          , scale_t<1000000ll*1000000ll,1*1>
		                          , value_type >;
		using quantity3 = quantity< unit< exponent_t<1+1+1>
		                                , exponent_t<2+2+2>
		                                , exponent_t<3+3+3>
		                                , exponent_t<4+4+4>
		                                , exponent_t<5+5+5>
		                                , exponent_t<6+6+6>
		                                , exponent_t<7+7+7>>
		                          , scale_t<1000000ll*1000000ll*1000000ll,1*1*1>
		                          , value_type >;
		using quantity_1 = quantity< unit< exponent_t<-1>
		                                 , exponent_t<-2>
		                                 , exponent_t<-3>
		                                 , exponent_t<-4>
		                                 , exponent_t<-5>
		                                 , exponent_t<-6>
		                                 , exponent_t<-7>>
		                           , scale_t<1000000,1>
		                           , value_type >;
		using quantity_2 = quantity< unit< exponent_t<-1-1>
		                                 , exponent_t<-2-2>
		                                 , exponent_t<-3-3>
		                                 , exponent_t<-4-4>
		                                 , exponent_t<-5-5>
		                                 , exponent_t<-6-6>
		                                 , exponent_t<-7-7>>
		                           , scale_t<1000000ll*1000000ll,1>
		                           , value_type >;

		quantity1 a{2};

		SUBCASE("integer exponents") {
			CHECK(typeid(unlib::pow< 3>(a)) == typeid(quantity3));
			CHECK(typeid(unlib::pow< 2>(a)) == typeid(quantity2));
			CHECK(typeid(unlib::pow< 1>(a)) == typeid(quantity1));
			CHECK(typeid(unlib::pow< 0>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<-1>(a)) == typeid(quantity_1));
			CHECK(typeid(unlib::pow<-2>(a)) == typeid(quantity_2));
		}
		SUBCASE("ratio exponents") {
			using quantity_r1  = quantity1;
			using quantity_r2  = quantity< unit< exponent_t<1,2>
			                                   , exponent_t<2,2>
			                                   , exponent_t<3,2>
			                                   , exponent_t<4,2>
			                                   , exponent_t<5,2>
			                                   , exponent_t<6,2>
			                                   , exponent_t<7,2>>
			                             , scale_t<1000>
			                             , value_type >;
			using quantity_r3  = quantity< unit< exponent_t<1,3>
			                                   , exponent_t<2,3>
			                                   , exponent_t<3,3>
			                                   , exponent_t<4,3>
			                                   , exponent_t<5,3>
			                                   , exponent_t<6,3>
			                                   , exponent_t<7,3>>
			                             , scale_t<100>
			                             , value_type >;
			using quantity_r_1 = quantity< unit< exponent_t<-1>
			                                   , exponent_t<-2>
			                                   , exponent_t<-3>
			                                   , exponent_t<-4>
			                                   , exponent_t<-5>
			                                   , exponent_t<-6>
			                                   , exponent_t<-7>>
			                             , scale_t<1000000>
			                             , value_type >;
			using quantity_r_2 = quantity< unit< exponent_t<-1,2>
			                                   , exponent_t<-2,2>
			                                   , exponent_t<-3,2>
			                                   , exponent_t<-4,2>
			                                   , exponent_t<-5,2>
			                                   , exponent_t<-6,2>
			                                   , exponent_t<-7,2>>
			                             , scale_t<1000>
			                             , value_type >;
			using quantity_r_3 = quantity< unit< exponent_t<-1,3>
			                                   , exponent_t<-2,3>
			                                   , exponent_t<-3,3>
			                                   , exponent_t<-4,3>
			                                   , exponent_t<-5,3>
			                                   , exponent_t<-6,3>
			                                   , exponent_t<-7,3>>
			                              , scale_t<100>
			                              , value_type >;

			CHECK(typeid(unlib::pow<ratio_t< 3, 1>>(a)) == typeid(quantity3));
			CHECK(typeid(unlib::pow<ratio_t< 2, 1>>(a)) == typeid(quantity2));
			CHECK(typeid(unlib::pow<ratio_t< 1, 1>>(a)) == typeid(quantity1));
			CHECK(typeid(unlib::pow<ratio_t< 0, 1>>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<ratio_t<-1, 1>>(a)) == typeid(quantity_1));
			CHECK(typeid(unlib::pow<ratio_t<-2, 1>>(a)) == typeid(quantity_2));

			CHECK(typeid(unlib::pow<ratio_t< 1, 3>>(a)) == typeid(quantity_r3));
			CHECK(typeid(unlib::pow<ratio_t< 1, 2>>(a)) == typeid(quantity_r2));
			CHECK(typeid(unlib::pow<ratio_t< 1, 1>>(a)) == typeid(quantity_r1));
			CHECK(typeid(unlib::pow<ratio_t< 0, 1>>(a)) == typeid(quantity0));
			CHECK(typeid(unlib::pow<ratio_t< 1,-1>>(a)) == typeid(quantity_r_1));
			CHECK(typeid(unlib::pow<ratio_t< 1,-2>>(a)) == typeid(quantity_r_2));
			CHECK(typeid(unlib::pow<ratio_t< 1,-3>>(a)) == typeid(quantity_r_3));
		}
	}

	SUBCASE("sqrt") {
		using quantity_2 = quantity<unit_a, scale_t<100,1>, value_type>;

		using unit_sqrt = unit<exponent_t<1,2>
		                      ,exponent_t<2,2>
		                      ,exponent_t<3,2>
		                      ,exponent_t<4,2>
		                      ,exponent_t<5,2>
		                      ,exponent_t<6,2>
		                      ,exponent_t<7,2>>;
		CHECK(typeid(unlib::sqrt_unit_t<unit_a>) == typeid(unit_sqrt));

		using quantity_sqrt = quantity<unit_sqrt, scale_t<10>, value_type>;
		CHECK(typeid(quantity_sqrt) == typeid(unlib::sqrt_quantity_t<quantity_2>));

		const auto sq_root_1 = unlib::sqrt(quantity_2{81});
		CHECK(typeid(sq_root_1) == typeid(quantity_sqrt));
		CHECK(sq_root_1.get() == doctest::Approx(9));

		const auto sq_root_2 = unlib::pow<unlib::ratio_t<1,2>>(quantity_2{81});
		CHECK(typeid(sq_root_2) == typeid(quantity_sqrt));
		CHECK(sq_root_2.get() == doctest::Approx(9));

		const auto sq_root_3 = unlib::pow<unlib::ratio_t<2,4>>(quantity_2{81});
		CHECK(typeid(sq_root_3) == typeid(quantity_sqrt));
		CHECK(sq_root_3.get() == doctest::Approx(9));
	}

	SUBCASE("cbrt") {
		using quantity_3 = quantity<unit_a, scale_t<1000,1>, value_type>;

		using unit_cbrt = unit<exponent_t<1,3>
		                      ,exponent_t<2,3>
		                      ,exponent_t<3,3>
		                      ,exponent_t<4,3>
		                      ,exponent_t<5,3>
		                      ,exponent_t<6,3>
		                      ,exponent_t<7,3>>;
		CHECK(typeid(unlib::cbrt_unit_t<unit_a>) == typeid(unit_cbrt));

		using quantity_cbrt = quantity<unit_cbrt, scale_t<10>, value_type>;
		CHECK(typeid(quantity_cbrt) == typeid(unlib::cbrt_quantity_t<quantity_3>));

		const auto cb_root_1 = unlib::cbrt(quantity_3{64});
		CHECK(typeid(cb_root_1) == typeid(quantity_cbrt));
		CHECK(cb_root_1.get() == doctest::Approx(4));

		const auto cb_root_2 = unlib::pow<unlib::ratio_t<1,3>>(quantity_3{64});
		CHECK(typeid(cb_root_2) == typeid(quantity_cbrt));
		CHECK(cb_root_2.get() == doctest::Approx(4));

		const auto cb_root_3 = unlib::pow<unlib::ratio_t<3,9>>(quantity_3{64});
		CHECK(typeid(cb_root_3) == typeid(quantity_cbrt));
		CHECK(cb_root_3.get() == doctest::Approx(4));
	}

}

TEST_CASE_TEMPLATE("is_near, is_smaller, is_greater", Float, float, double, long double) {

	const auto f_tiny        = Float{  0.0000001f};
	const auto f_0_0         = Float{  0.0f      };
	const auto f_0_001       = Float{  0.001f    };
	const auto f_0_01        = Float{  0.01f     };
	const auto f_1_0         = Float{  1.0f      };
	const auto f_1_1         = Float{  1.1f      };
	const auto f_3_0         = Float{  3.0f      };
	const auto f_49_99999    = Float{ 49.99999f  };
	const auto f_50_0        = Float{ 50.0f      };
	const auto f_50_1        = Float{ 50.1f      };
	const auto f_100_0       = Float{100.0f      };
	const auto f_150_0       = Float{150.0f      };
	const auto f_1000_0      = Float{1000.0f     };
	const auto f_epsilon     = std::numeric_limits<Float>::epsilon();
	const auto f_2_epsilon   = f_epsilon * 2;
	const auto tolerance_def_nominal  = static_cast<Float>(unlib::tolerance_default_nominal );
	const auto tolerance_def_fraction = static_cast<Float>(unlib::tolerance_default_fraction);

	const auto frac_0_001 = unlib::fraction<Float>{f_0_001};

	using unlib::fraction;

	const comparison_table_entry<Float> comparison_table[] =
		//                               |-----------comparison_table_result----------|-------------operands------------|------tolerances------|
		//                                value, nominal, fraction, frac&nom                lhs,     rhs                , tol1       ,   tol2
		{ comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_1_0,   f_1_0              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_0_0,   f_0_0              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__},     f_1_0,   f_1_0 + f_epsilon  , f_2_epsilon,   f_1_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__},   f_100_0, f_150_0              , f_50_1     ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_0,   f_1_1              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_1,   f_1_0              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_0,   f_1_0 + f_2_epsilon, f_epsilon  ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},   f_100_0, f_150_0              , f_49_99999 ,   f_1_0 }
		, comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_1_0,   f_1_0              , f_tiny     ,   f_3_0 }
		, comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_0_0,   f_0_0              , f_tiny     ,   f_3_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__},     f_1_0,   f_1_0 + f_epsilon  , f_2_epsilon,   f_3_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__},   f_100_0, f_150_0              , f_50_1     ,   f_3_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_0,   f_1_1              , f_tiny     ,   f_3_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_1,   f_1_0              , f_tiny     ,   f_3_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , true   , __LINE__},     f_1_0,   f_1_0 + f_2_epsilon, f_epsilon  ,   f_3_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , true   , __LINE__},   f_100_0, f_150_0              , f_49_99999 ,   f_3_0 }
		, comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_1_0,   f_1_0              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__},     f_0_0,   f_0_0              , f_tiny     ,   f_0_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__},     f_1_0,   f_1_0 + f_epsilon  , f_2_epsilon,   f_1_0 }
		, comparison_table_entry<Float>{ {true , false  , true    , false  , __LINE__},   f_100_0, f_150_0              , f_50_1     , f_0_001 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_0,   f_1_1              , f_tiny     ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_1,   f_1_0              , f_tiny     ,   f_1_1 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},     f_1_0,   f_1_0 + f_2_epsilon, f_epsilon  ,   f_1_0 }
		, comparison_table_entry<Float>{ {false, false  , false   , false  , __LINE__},   f_100_0, f_150_0              , f_49_99999 , f_0_001 }
		};

	const zero_comparison_table_entry<Float> zero_comparison_table[] =
		//                                    |----------comparison_table_result----------|--value---|----tolerances----|
		//                                     value, nominal, fraction, frac&nom                      tol1  , tol2
		{ zero_comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__}, f_0_0   , f_tiny, f_1_0     }
		, zero_comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__}, f_0_001 , f_0_01, f_1_0     }
		, zero_comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__}, f_0_0   , f_tiny, f_3_0     }
		, zero_comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__}, f_0_001 , f_0_01, f_3_0     }
		, zero_comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__}, f_0_0   , f_tiny, f_50_0    }
		, zero_comparison_table_entry<Float>{ {true , false  , true    , true   , __LINE__}, f_0_001 , f_0_01, f_50_0    }
		, zero_comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__}, f_0_0   , f_tiny, f_tiny    }
		, zero_comparison_table_entry<Float>{ {true , false  , true    , false  , __LINE__}, f_0_001 , f_0_01, f_tiny    }
		, zero_comparison_table_entry<Float>{ {true , true   , true    , true   , __LINE__}, f_0_0   , f_tiny, f_epsilon }
		, zero_comparison_table_entry<Float>{ {true , false  , true    , false  , __LINE__}, f_0_001 , f_0_01, f_epsilon }
		};

	SUBCASE("tolerances") {
		using Watt = unlib::watt<Float>;
		SUBCASE("types") {
			CHECK( typeid(unlib::tolerance_value   <Float>()) == typeid(unlib::tolerance_val<Float>) );
			CHECK( typeid(unlib::tolerance_value   <Watt >()) == typeid(unlib::tolerance_val<Watt >) );
			CHECK( typeid(unlib::tolerance_nominal <Float>()) == typeid(unlib::tolerance_nom<Float>) );
			CHECK( typeid(unlib::tolerance_nominal <Watt >()) == typeid(unlib::tolerance_nom<Watt >) );
			CHECK( typeid(unlib::tolerance_fraction<Float>()) == typeid(unlib::tolerance_frc<fraction<Float>,Float>) );
			CHECK( typeid(unlib::tolerance_fraction<Watt >()) == typeid(unlib::tolerance_frc<fraction<Float>,Watt >) );
		}
		SUBCASE("values") {
			CHECK( unlib::tolerance_value           (     f_0_001    ).v       == doctest::Approx(     f_0_001   ) );
			CHECK( unlib::tolerance_value           (Watt{f_0_001   }).v.get() == doctest::Approx(Watt{f_0_001   }.get()) );
			CHECK( unlib::tolerance_nominal         (     f_1000_0   ).v       == doctest::Approx(     f_1000_0  ) );
			CHECK( unlib::tolerance_nominal         (Watt{f_1000_0  }).v.get() == doctest::Approx(Watt{f_1000_0  }.get()) );
			CHECK( unlib::tolerance_fraction<Float>(                 frac_0_001 ).v.get() == doctest::Approx(                frac_0_001 .get()) );
			CHECK( unlib::tolerance_fraction<Watt  >(fraction<Float>{f_1000_0  }).v.get() == doctest::Approx(fraction<Float>{f_1000_0  }.get()) );
			SUBCASE("default values") {
				CHECK( unlib::tolerance_value   <Float>().v       == doctest::Approx(     tolerance_def_nominal*tolerance_def_fraction ) );
				CHECK( unlib::tolerance_value   <Watt >().v.get() == doctest::Approx(Watt{tolerance_def_nominal*tolerance_def_fraction}.get()) );
				CHECK( unlib::tolerance_nominal <Float>().v       == doctest::Approx(     tolerance_def_nominal ) );
				CHECK( unlib::tolerance_nominal <Watt >().v.get() == doctest::Approx(Watt{tolerance_def_nominal}.get()) );
				CHECK( unlib::tolerance_fraction<Float>().v.get() == doctest::Approx(fraction<Float>{tolerance_def_fraction}.get()) );
				CHECK( unlib::tolerance_fraction<Watt >().v.get() == doctest::Approx(fraction<Float>{tolerance_def_fraction}.get()) );
			}
		}
	}

	SUBCASE("comparison between two numbers") {
		for(auto& entry : comparison_table) {
			comparison_tests<Float>::is_near   (entry);
			comparison_tests<Float>::is_smaller(entry);
			comparison_tests<Float>::is_greater(entry);
		}
	}
	SUBCASE("comparison with zero") {
		for(auto& entry : zero_comparison_table) {
			comparison_tests<Float>::is_near   (entry);
			comparison_tests<Float>::is_smaller(entry);
			comparison_tests<Float>::is_greater(entry);
		}
	}

}
