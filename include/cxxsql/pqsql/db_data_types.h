#pragma once

#include "detail/db_type.h"
#include <cxxsql/db_data_types.h>

namespace cxxsql::pgsql
{
  template<unsigned N = 0>
  requires requires { N == 0; }
  using smallserial = cxxsql::detail::db_type<detail::db_type_e::smallserial, N>;
  
  template<unsigned N = 0>
  requires requires { N == 0; }
  using serial = cxxsql::detail::db_type<detail::db_type_e::serial, N>;
  
  template<unsigned N = 0>
  requires requires { N == 0; }
  using bigserial = cxxsql::detail::db_type<detail::db_type_e::bigserial, N>;
  
  template<unsigned N = 0>
  using boolean = cxxsql::detail::db_type<detail::db_type_e::boolean, N>;
  
  template<unsigned N = 0>
  using double_precision = cxxsql::detail::db_type<detail::db_type_e::double_precision, N>;
}
