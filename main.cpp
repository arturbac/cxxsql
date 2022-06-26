#include <cxxsql/api.h>
#include <iostream>

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
    
// using test_table2_t = 
//     table_t<"test_table2",
//       column_t<"id",int32<>>,
//       column_t<"id",int16<>>
//       >;
      
static constexpr auto create_statement_test_table = create_table_statement<test_table_t>();
}


  
static constexpr auto tst_conv{ strconv::int2str<32,
                  strconv::integral_format_traits{ .precision = 12, 
                                                   .format = strconv::format_e::hexadecimal,
                                                   .char_case = strconv::char_case_e::uppercase,
                                                   .padd_with = strconv::padd_with_e::space,
                                                   .sign = strconv::prepend_sign_e::always,
                                                   .alignment = strconv::alignment_e::middle
                                                    }>() };
int main(int argc, char const * const * argv) 
{
  cxxsql::test_table_t::record_type rec{ 3,{'c','x','t'},{{'a','b'}}, std::nullopt, "text", {0x24u,0x23u,0x25u},{0x0u,0x1u,0x2u,0x3u} };

  std::cout << cxxsql::create_statement_test_table.value().view() << std::endl;
  
  std::cout << "|" << tst_conv.view() << "|" << std::endl;
//   __builtin_dump_struct(&rec, &printf);
  return 0;
}
