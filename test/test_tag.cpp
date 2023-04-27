#include <unlib/tag.hpp>

#include <doctest/doctest.h>

#include <unlib/test/unlib_test.hpp>

namespace {

using tag_1  = unlib::tag_t<struct tag_id_1>;
using tag_2  = unlib::tag_t<struct tag_id_2>;
using tag_3a = unlib::tag_t<struct tag_id_3>;
using tag_3b = unlib::tag_t<tag_3a::id>;
using tag_4  = unlib::tag_t<void>;

}

TEST_CASE("tag") {
	SUBCASE("compatibility") {
		SUBCASE("comparable") {
			CHECK( not unlib::are_tags_comparable_v<tag_1 , tag_2 > );
			CHECK( not unlib::are_tags_comparable_v<tag_1 , tag_3a> );
			CHECK( not unlib::are_tags_comparable_v<tag_1 , tag_3b> );
			CHECK( not unlib::are_tags_comparable_v<tag_1 , tag_4 > );
			CHECK( not unlib::are_tags_comparable_v<tag_2 , tag_1 > );
			CHECK( not unlib::are_tags_comparable_v<tag_2 , tag_3a> );
			CHECK( not unlib::are_tags_comparable_v<tag_2 , tag_3b> );
			CHECK( not unlib::are_tags_comparable_v<tag_2 , tag_4 > );
			CHECK( not unlib::are_tags_comparable_v<tag_3a, tag_1 > );
			CHECK( not unlib::are_tags_comparable_v<tag_3b, tag_1 > );
			CHECK( not unlib::are_tags_comparable_v<tag_3a, tag_2 > );
			CHECK( not unlib::are_tags_comparable_v<tag_3b, tag_2 > );
			CHECK(     unlib::are_tags_comparable_v<tag_3a, tag_3b> );
			CHECK(     unlib::are_tags_comparable_v<tag_3b, tag_3a> );
			CHECK( not unlib::are_tags_comparable_v<tag_3a, tag_4 > );
			CHECK( not unlib::are_tags_comparable_v<tag_3b, tag_4 > );
		}
		SUBCASE("multipliable") {
			CHECK( not unlib::are_tags_multipliable_v<tag_1 , tag_2 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_1 , tag_3a> );
			CHECK( not unlib::are_tags_multipliable_v<tag_1 , tag_3b> );
			CHECK(     unlib::are_tags_multipliable_v<tag_1 , tag_4 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_2 , tag_1 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_2 , tag_3a> );
			CHECK( not unlib::are_tags_multipliable_v<tag_2 , tag_3b> );
			CHECK(     unlib::are_tags_multipliable_v<tag_2 , tag_4 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_3a, tag_1 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_3b, tag_1 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_3a, tag_2 > );
			CHECK( not unlib::are_tags_multipliable_v<tag_3b, tag_2 > );
			CHECK(     unlib::are_tags_multipliable_v<tag_3a, tag_3b> );
			CHECK(     unlib::are_tags_multipliable_v<tag_3b, tag_3a> );
			CHECK(     unlib::are_tags_multipliable_v<tag_3a, tag_4 > );
			CHECK(     unlib::are_tags_multipliable_v<tag_3b, tag_4 > );
		}
		SUBCASE("castable") {
			CHECK( not unlib::are_tags_castable_v<tag_1 , tag_2 > );
			CHECK( not unlib::are_tags_castable_v<tag_1 , tag_3a> );
			CHECK( not unlib::are_tags_castable_v<tag_1 , tag_3b> );
			CHECK( not unlib::are_tags_castable_v<tag_1 , tag_4 > );
			CHECK( not unlib::are_tags_castable_v<tag_2 , tag_1 > );
			CHECK( not unlib::are_tags_castable_v<tag_2 , tag_3a> );
			CHECK( not unlib::are_tags_castable_v<tag_2 , tag_3b> );
			CHECK( not unlib::are_tags_castable_v<tag_2 , tag_4 > );
			CHECK( not unlib::are_tags_castable_v<tag_3a, tag_1 > );
			CHECK( not unlib::are_tags_castable_v<tag_3b, tag_1 > );
			CHECK( not unlib::are_tags_castable_v<tag_3a, tag_2 > );
			CHECK( not unlib::are_tags_castable_v<tag_3b, tag_2 > );
			CHECK(     unlib::are_tags_castable_v<tag_3a, tag_3b> );
			CHECK(     unlib::are_tags_castable_v<tag_3b, tag_3a> );
			CHECK( not unlib::are_tags_castable_v<tag_3a, tag_4 > );
			CHECK( not unlib::are_tags_castable_v<tag_3b, tag_4 > );
		}
	}
}
