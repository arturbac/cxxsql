# cxxsql

Designing modern c++20 sql constevaluated api

## goals

 - generate select, insert, create table, create indexes etc at compile time
 - strong type verification 
 
## designing stage

 -  flexible syntax mimicking sql table declaration
```C++
#include <cxxsql/api.h>

namespace cxxsql
{
using test_table_t = 
table_decl_t<"test_table",
  column_t<"id",db_type::int32<>>,
  column_t<"name",db_type::char8<3>, nullable_e::not_null>,
  column_t<"nameopt",db_type::char8<3>, nullable_e::null>,
  column_t<"oid",db_type::int32<>, nullable_e::null>,
  column_t<"descr",db_type::varchar, nullable_e::null>,
  column_t<"data",db_type::varbinary, nullable_e::not_null>,
  column_t<"data2",db_type::binary<5>, nullable_e::not_null>
  >;
}

cxxsql::test_table_t::record_type
   rec{ 3,{'c','x','t'},{{'a','b'}}, std::nullopt, "text", {0x24u,0x23u,0x25u},{0x0u,0x1u,0x2u,0x3u} };
```

 - constraint on column name uniqueness
 
```C++
#include <cxxsql/api.h>

namespace cxxsql
{
using test_table2_t = 
table_decl_t<"test_table2",
  column_t<"id",db_type::int32<>>,
  column_t<"id",db_type::int16<>>
  >;
  
cxxsql::test_table2_t::record_type rec2 {};
```

```bash
cxx20sql/include/cxxsql/api.h:163:32: error: constraints not satisfied for class template 'row_decl_t' 
  [with Members = <cxxsql::column_t<{{{105, 100, 0}}}, cxxsql::detail::db_type<cxxsql::db_type_e::int32, 0>, cxxsql::nullable_e::not_null>,
                   cxxsql::column_t<{{{105, 100, 0}}}, cxxsql::detail::db_type<cxxsql::db_type_e::int16, 0>, cxxsql::nullable_e::not_null>>]
  struct table_decl_t : public row_decl_t<Members ...>
                               ^~~~~~~~~~~~~~~~~~~~~~~
                               
```
