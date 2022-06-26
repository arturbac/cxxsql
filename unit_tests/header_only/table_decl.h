#pragma once

#include "cxxsql/unit_test_common.h"

namespace cxxsql
{

[[nodiscard]]
constexpr bool table_decl_unit_tests() noexcept
  {
  using test_table_t = 
    table_t<"test_table",
      column_t<"id", int32<>>,
      column_t<"name", char8<3>>,
      column_t<"nameopt", char8<3>, null>,
      column_t<"oid", int32<>, null>,
      column_t<"descr", varchar, null>,
      column_t<"data", varbinary>,
      column_t<"data2", binary<5>>
      >;
      
  return !test_table_t::name().value().view().empty();
  }
}
