#include <cxxsql/api.h>

namespace cxxsql
{
using test_table_t = 
table_decl_t<"test_table",
  column_t<"id",db_type::int32<>>,
  column_t<"name",db_type::char8<3>>,
  column_t<"nameopt",db_type::char8<3>, nullable_e::null>,
  column_t<"oid",db_type::int32<>, nullable_e::null>,
  column_t<"descr",db_type::varchar, nullable_e::null>,
  column_t<"data",db_type::varbinary>,
  column_t<"data2",db_type::binary<5>>
  >;
  
// using test_table2_t = 
// table_decl_t<"test_table2",
//   column_t<"id",db_type::int32<>>,
//   column_t<"id",db_type::int16<>>
//   >;
}

int main(int argc, char **argv) 
{
  cxxsql::test_table_t::record_type rec{ 3,{'c','x','t'},{{'a','b'}}, std::nullopt, "text", {0x24u,0x23u,0x25u},{0x0u,0x1u,0x2u,0x3u} };
  
  //supposed to fail at compile time
//   cxxsql::test_table2_t::record_type rec2 {};
  
//   std::cout << "Table mem layout :" << std::endl;
//   __builtin_dump_struct(&rec, &printf);
  return 0;
}
