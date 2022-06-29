#pragma once

#include "cxxsql/unit_test_common.h"

namespace cxxsql
{

[[nodiscard]]
constexpr bool fail_column_constraints_unit_tests() noexcept
  {
  using test_table_t = 
    table_t<"test_table",
      column_t<"id", int32<>, constraints_t<constraints::not_null,
                                            constraints::primary_key,
                                            constraints::null>>
      >;
  return !test_table_t::name().value().view().empty();
  }
}
