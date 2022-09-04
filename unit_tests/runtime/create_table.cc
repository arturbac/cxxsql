#include <boost/ut.hpp>
#include <cxxsql/api.h>

int main() {
  using namespace boost::ut;
  using namespace cxxsql;
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
  static constexpr auto create_statement_test_table = create_table_statement<test_table_t>();
  
  static_assert( create_statement_test_table.value().view() == 
R"(CREATE TABLE test_table (
	"id" int NOT NULL PRIMARY KEY,
	"name" char(3) NOT NULL,
	"nameopt" char(3) NULL UNIQUE,
	"oid" int NULL,
	"descr" varchar NULL,
	"data" varbinary NOT NULL,
	"data2" binary(5) NOT NULL
	))"
  );

  }

  {
  using test_table_t = 
    table_t<"test_table2",
      column_t<"id", int32<>, constraints_t<constraints::not_null,
                                            constraints::primary_key>>
      >;
  static constexpr auto create_statement_test_table = create_table_statement<test_table_t>();
  static_assert( create_statement_test_table.value().view() == 
R"(CREATE TABLE test_table2 (
	"id" int NOT NULL PRIMARY KEY
	))");
  }
  
  
  
}
