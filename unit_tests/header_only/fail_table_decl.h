#pragma once

#include "cxxsql/unit_test_common.h"

namespace cxxsql
{

[[nodiscard]]
constexpr bool fail_table_decl_unit_tests() noexcept
  {
  using test_table_t = 
    table_t<"test_table",
      column_t<"id", int32<>, constraints_t<constraints::not_null,
                                            constraints::primary_key>>,
      column_t<"name", char8<3>>,
      column_t<"nameopt", char8<3>, constraints_t<constraints::null,
                                                  constraints::unique>>,
      column_t<"oid", int32<>, constraints_t<constraints::null>>,
      column_t<"id", int32<>>,
      column_t<"descr", varchar, constraints_t<constraints::null>>,
      column_t<"data", varbinary>,
      column_t<"data2", binary<5>>
      >;

  return !test_table_t::name().value().view().empty();
  }
}
