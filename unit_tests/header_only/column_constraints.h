#pragma once

#include "cxxsql/unit_test_common.h"

namespace cxxsql
{
  
namespace test_concepts
{
struct x{};

using cl0 = constraints_t<constraints::not_null,
                          constraints::primary_key>;

using test_table_t = 
  table_t<"test_table",
    column_t<"id", int32<>, constraints_t<constraints::not_null,
                                          constraints::primary_key>>
        >;
consteval void test()
  {
  CONSTEXPR_TEST(concepts::constraint_list<cl0>);
  CONSTEXPR_TEST(!concepts::constraint_list<int>);
  CONSTEXPR_TEST(!concepts::constraint_list<x>);
  CONSTEXPR_TEST(!concepts::constraint_list<test_table_t>);
  }
}
namespace test_has_column_constraint
{
using t0 = constraints_t<constraints::not_null, constraints::primary_key>;
using t1 = constraints_t<constraints::null>;
consteval bool test()
  {
  CONSTEXPR_TEST(detail::has_constraint<t0,constraints::not_null>());
  CONSTEXPR_TEST(!detail::has_constraint<t0,constraints::null>());
  CONSTEXPR_TEST(detail::has_constraint<t0,constraints::primary_key>());
  
  CONSTEXPR_TEST(!detail::has_constraint<t1,constraints::not_null>());
  CONSTEXPR_TEST(detail::has_constraint<t1,constraints::null>());
  CONSTEXPR_TEST(!detail::has_constraint<t1,constraints::primary_key>());
  return true;
  }
}
namespace test_null_column_constraint
{
using t0 = constraints_t<constraints::not_null, constraints::primary_key>;
using t1 = constraints_t<constraints::null>;
using t2 = constraints_t<constraints::not_null, constraints::primary_key,constraints::null>;
using t3 = constraints_t<constraints::primary_key>;
consteval void test()
  {
  CONSTEXPR_TEST(concepts::unique_nullability<t0>);
  CONSTEXPR_TEST(concepts::unique_nullability<t1>);
  CONSTEXPR_TEST(!concepts::unique_nullability<t2>);
  CONSTEXPR_TEST(!concepts::unique_nullability<t3>);
  }
}

[[nodiscard]]
consteval bool column_constraints_unit_tests()
  {
  test_concepts::test();
  test_has_column_constraint::test();
  test_null_column_constraint::test();
  return true;
  }

}
