#include <cxxsql/api.h>
#include <iostream>

namespace cxxsql
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
    
// using test_table2_t = 
//     table_t<"test_table2",
//       column_t<"id",int32<>>,
//       column_t<"id",int16<>>
//       >;
      
static constexpr auto create_statment_test_table = create_table_statment<test_table_t>();
}

int main(int argc, char const * const * argv) 
{
  cxxsql::test_table_t::record_type rec{ 3,{'c','x','t'},{{'a','b'}}, std::nullopt, "text", {0x24u,0x23u,0x25u},{0x0u,0x1u,0x2u,0x3u} };

  std::cout << cxxsql::create_statment_test_table.view() << std::endl;
//   __builtin_dump_struct(&rec, &printf);
  return 0;
}
