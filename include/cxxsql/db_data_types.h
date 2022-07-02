#pragma once

#include "detail/db_type.h"

namespace cxxsql
{

  template<unsigned N = 0>
  using char8 = detail::db_type<detail::db_type_e::char8, N>;
  
  template<unsigned N = 0>
  using int16 = detail::db_type<detail::db_type_e::int16, N>;
  
  template<unsigned N = 0>
  using int32 = detail::db_type<detail::db_type_e::int32, N>;
  
  template<unsigned N = 0>
  using int64 = detail::db_type<detail::db_type_e::int64, N>;
  
  using varchar = detail::db_type<detail::db_type_e::varchar>;
  
  template<unsigned N>
  requires requires { N != 0; }
  using binary = detail::db_type<detail::db_type_e::binary, N>;
  
  using varbinary = detail::db_type<detail::db_type_e::varbinary>;
  }
