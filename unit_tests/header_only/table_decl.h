#pragma once

#include "cxxsql/unit_test_common.h"

namespace cxxsql
{
using test_table_t = 
    table_t<"test_table",
      column_t<"id", int32<>, constraints_t<constraints::not_null,
                                            constraints::primary_key>>,
      column_t<"name", char8<3>>,
      column_t<"nameopt", char8<3>, constraints_t<constraints::null,
                                                  constraints::unique>>,
      column_t<"oid", int32<>, constraints_t<constraints::null>>,
      column_t<"descr", varchar, constraints_t<constraints::null>>,
      column_t<"data", varbinary>,
      column_t<"data2", binary<5>>
      >;
      
   constexpr auto names{ column_names<test_table_t>() };
   
// constexpr auto names { column_name_array<test_table_t>() };

[[nodiscard]]
consteval bool table_decl_unit_tests() noexcept
  {
  CONSTEXPR_TEST(!test_table_t::name().value().view().empty());
  
  using std::string_view_literals::operator""sv;

  CONSTEXPR_TEST(std::get<0>(names).view() == "id"sv);
  CONSTEXPR_TEST(std::get<1>(names).view() == "name"sv);
  CONSTEXPR_TEST(std::get<2>(names).view() == "nameopt"sv);
  CONSTEXPR_TEST(std::get<3>(names).view() == "oid"sv);
  CONSTEXPR_TEST(std::get<4>(names).view() == "descr"sv);
  CONSTEXPR_TEST(std::get<5>(names).view() == "data"sv);
  CONSTEXPR_TEST(std::get<6>(names).view() == "data2"sv);
  return true;
  }
}
