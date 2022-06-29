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
      
}

cxxsql::test_table_t::record_type
   rec{ 3,{'c','x','t'},{{'a','b'}}, std::nullopt, "text", {0x24u,0x23u,0x25u},{0x0u,0x1u,0x2u,0x3u} };
   

```

 - example generate select (wip)
 
```C++
namespace cxxsql
{
static constexpr auto create_statment_test_table = create_table_statment<test_table_t>();
}
std::cout << cxxsql::create_statment_test_table.view() << std::endl;
```

outputs const evaluated string literal

```SQL
CREATE TABLE test_table (
	"id" int NOT NULL PRIMARY KEY,
	"name" char[3] NOT NULL,
	"nameopt" char[3] NULL UNIQUE,
	"oid" int NULL,
	"descr" varchar NULL,
	"data" varbinary NOT NULL,
	"data2" binary[5] NOT NULL
	)
```

 - constraint on column name uniqueness
 
```C++
#include <cxxsql/api.h>

namespace cxxsql
{

using test_table2_t = 
    table_t<"test_table2",
      column_t<"id",int32<>>,
      column_t<"id",int16<>>
      >;
```

```bash
cxx20sql/main.cpp:19:5: error: constraints not satisfied for class template 'table_t' 
  [with str = {{116, 101, 115, 116, 95, 116, 97, 98, 108, 101, ...}},
        Members = <cxxsql::column_t<{{{105, 100, 0}}}, cxxsql::detail::db_type<cxxsql::detail::db_type_e::int32, 0>, cxxsql::detail::nullable_e::not_null>,
                   cxxsql::column_t<{{{105, 100, 0}}}, cxxsql::detail::db_type<cxxsql::detail::db_type_e::int16, 0>, cxxsql::detail::nullable_e::not_null>>]
    table_t<"test_table2",
    ^~~~~~~~~~~~~~~~~~~~~~
cxx20sql/include/cxxsql/api.h:150:22: note: because 'concepts::must_be_unique_column_name<cxxsql::column_t<{{{105, 100, 0}}},
  cxxsql::detail::db_type<cxxsql::detail::db_type_e::int32, 0>, cxxsql::detail::nullable_e::not_null>,
  cxxsql::column_t<{{{105, 100, 0}}}, cxxsql::detail::db_type<cxxsql::detail::db_type_e::int16, 0>, cxxsql::detail::nullable_e::not_null> >'
   evaluated to false
  requires concepts::must_be_unique_column_name<Members...>
                     ^
cxx20sql/include/cxxsql/api.h:139:42: note: because 'detail::must_be_unique_column_name<cxxsql::column_t<{{{105, 100, 0}}},
  cxxsql::detail::db_type<cxxsql::detail::db_type_e::int32, 0>, cxxsql::detail::nullable_e::not_null>, cxxsql::column_t<{{{105, 100, 0}}},
  cxxsql::detail::db_type<cxxsql::detail::db_type_e::int16, 0>, cxxsql::detail::nullable_e::not_null> >()'
   evaluated to false
    concept must_be_unique_column_name = detail::must_be_unique_column_name<Members...>();
```

 - constraint on column sql constraints like null/not_null, prevents invalid table declaration
 
```C++
#include <cxxsql/api.h>
  
using test_table_t = 
  table_t<"test_table",
    column_t<"id", int32<>, constraints_t<constraints::not_null,
                                          constraints::primary_key,
                                          constraints::null>>
    >;
```

```bash
unit_tests/header_only/fail_column_constraints.h:13:7: error: constraints not satisfied for class template 'column_t' [with nm = {{{{105, 100, 0}}}}, dbtype = cxxsql::detail::db_type<cxxsql::detail::db_type_e::int32, 0>, cstrs = cxxsql::constraints_t<cxxsql::constraint<{{78, 79, 84, 32, 78, 85, 76, 76, 0}}, {{{{0}}}}>, cxxsql::constraint<{{80, 82, 73, 77, 65, 82, 89, 32, 75, 69, ...}}, {{{{0}}}}>, cxxsql::constraint<{{78, 85, 76, 76, 0}}, {{{{0}}}}>>]
      column_t<"id", int32<>, constraints_t<constraints::not_null,
      ^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cxxsql/column.h:148:22: note: because 'cxxsql::constraints_t<cxxsql::constraint<{{78, 79, 84, 32, 78, 85, 76, 76, 0}}, {{{{0}}}}>, cxxsql::constraint<{{80, 82, 73, 77, 65, 82, 89, 32, 75, 69, ...}}, {{{{0}}}}>, cxxsql::constraint<{{78, 85, 76, 76, 0}}, {{{{0}}}}>>' does not satisfy 'valid_column_constraints'
           concepts::valid_column_constraints cstrs = constraints_t<constraints::not_null>
                     ^
cxxsql/column.h:143:26: note: because 'cxxsql::constraints_t<cxxsql::constraint<{{78, 79, 84, 32, 78, 85, 76, 76, 0}}, {{{{0}}}}>, cxxsql::constraint<{{80, 82, 73, 77, 65, 82, 89, 32, 75, 69, ...}}, {{{{0}}}}>, cxxsql::constraint<{{78, 85, 76, 76, 0}}, {{{{0}}}}>>' does not satisfy 'unique_nullability'
      requires concepts::unique_nullability<cstrs>;
                         ^
cxxsql/column.h:107:16: note: because 'detail::has_constraint<cxxsql::constraints_t<cxxsql::constraint<{{78, 79, 84, 32, 78, 85, 76, 76, 0}}, {{{{0}}}}>, cxxsql::constraint<{{80, 82, 73, 77, 65, 82, 89, 32, 75, 69, ...}}, {{{{0}}}}>, cxxsql::constraint<{{78, 85, 76, 76, 0}}, {{{{0}}}}> >, constraints::not_null>() != detail::has_constraint<cxxsql::constraints_t<cxxsql::constraint<{{78, 79, 84, 32, 78, 85, 76, 76, 0}}, {{{{0}}}}>, cxxsql::constraint<{{80, 82, 73, 77, 65, 82, 89, 32, 75, 69, ...}}, {{{{0}}}}>, cxxsql::constraint<{{78, 85, 76, 76, 0}}, {{{{0}}}}> >, constraints::null>()' (1 != 1) evaluated to false
      requires detail::has_constraint<cstrs,constraints::not_null>() !=  detail::has_constraint<cstrs,constraints::null>();
               ^
```

## todos

 - compile time sql syntax generators (psql/mssql/slite)
 - extended options for tables
