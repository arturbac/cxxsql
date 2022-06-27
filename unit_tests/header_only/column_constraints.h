#include <cxxsql/api.h>

namespace cxxsql
{
  
struct x{};

using cl0 = constraints_t<constraints::not_null,
                          constraints::primary_key>;

using test_table_t = 
  table_t<"test_table",
    column_t<"id", int32<>, constraints_t<constraints::not_null,
                                          constraints::primary_key>>
        >;
[[nodiscard]]
consteval bool column_constraints_unit_tests() noexcept
  {
  static_assert(concepts::constraint_list<cl0>);
  static_assert(!concepts::constraint_list<int>);
  static_assert(!concepts::constraint_list<x>);
  static_assert(!concepts::constraint_list<test_table_t>);
  return true;
  }

}
