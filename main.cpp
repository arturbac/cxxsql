#include <cxxsql/api.h>
#include <cxxsql/pqsql/db_data_types.h>
#include <cxxsql/pqsql/connection.h>

#include <iostream>
#include <array>

namespace cxxsql
{

using namespace constraints;

template<typename ...Members>
using cc = constraints_t<Members ...>;

using test_table_t = 
    table_t<"test_table",
      column_t<"id",      int32<>,   cc<not_null,primary_key>>,
      column_t<"name",    char8<3>>,
      column_t<"nameopt", char8<3>,  cc<null,unique>>,
      column_t<"oid",     int32<>,   cc<null>>,
      column_t<"descr",   varchar,   cc<null>>,
      column_t<"data",    varbinary>,
      column_t<"data2",   binary<5>>
      >;
    
// using test_table2_t = 
//     table_t<"test_table2",
//       column_t<"id",int32<>>,
//       column_t<"id",int16<>>
//       >;
      
static constexpr auto create_statement_test_table = create_table_statement<test_table_t>();

using test_table_pgsql_t = 
    table_t<"test_table_pgsql",
      column_t<"id",    pgsql::serial<>, cc<not_null,primary_key>>,
      column_t<"name",  char8<3>>,
      column_t<"foatval",  pgsql::double_precision<>>,
      column_t<"boolval",  pgsql::boolean<>>
      >;
      
static constexpr auto create_statement_test_table_pgsql = create_table_statement<test_table_pgsql_t>();
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

  std::cout << "universal :\n" << cxxsql::create_statement_test_table.value().view() << std::endl;
  
  std::cout << "pgsql :\n" << cxxsql::create_statement_test_table_pgsql.value().view() << std::endl;
  
  std::cout << "|" << tst_conv.view() << "|" << std::endl;
//   __builtin_dump_struct(&rec, &printf);
  using std::string_view_literals::operator""sv;

  constexpr std::array open_params{
              std::pair{"host"sv,"localhost"sv},
              std::pair{"port"sv,"5432"sv},
              std::pair{"dbname"sv,"postgres"sv},
              std::pair{"user"sv,"postgres"sv}
          };
    {
    cxxsql::pgsql::connection_t conn{cxxsql::pgsql::open( open_params)};

    if(conn)
      {
        std::cout << "Connected " <<std::endl;
      }
    else
      {
      auto status { conn.status() };
      std::cout << "Not connected "<< status.message() << " " << status.description() <<std::endl;
      }
    }
  return 0;
}
