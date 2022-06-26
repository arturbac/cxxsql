#include <boost/ut.hpp>
#include <cxxsql/api.h>

int main() {
  using namespace boost::ut;
  using namespace cxxsql;
  
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
  static constexpr auto create_statement_test_table = create_table_statement<test_table_t>();
  
  expect( create_statement_test_table.value().view() == 
R"(CREATE TABLE test_table (
	"id" int not_null,
	"name" char[3] not_null,
	"nameopt" char[3] null,
	"oid" int null,
	"descr" varchar null,
	"data" varbinary not_null,
	"data2" binary[5] not_null
	))"
  );
  
}
