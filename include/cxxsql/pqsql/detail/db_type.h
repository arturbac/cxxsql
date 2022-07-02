#pragma once

#include "common.h"
#include <cxxsql/detail/db_type.h>

namespace cxxsql::detail
{
  template<>
  struct map_db_type<cxxsql::pgsql::detail::db_type_e::smallserial, 0, nullable_e::not_null>
    { 
    using type = int16_t;
    static constexpr basic_fixed_string db_string {"smallserial"};
    };
    
  template<>
  struct map_db_type<cxxsql::pgsql::detail::db_type_e::serial, 0, nullable_e::not_null>
    { 
    using type = int32_t;
    static constexpr basic_fixed_string db_string {"serial"};
    };
    
  template<>
  struct map_db_type<cxxsql::pgsql::detail::db_type_e::bigserial, 0, nullable_e::not_null>
    { 
    using type = int64_t;
    static constexpr basic_fixed_string db_string {"bigserial"};
    };
    
  template<cxxsql::pgsql::detail::db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::null> 
    {
    using related_type = map_db_type<dbtype, N, nullable_e::not_null>;
    using type = std::optional<typename related_type::type>;
    static constexpr basic_fixed_string db_string { related_type::db_string };
    };
    
  template<cxxsql::pgsql::detail::db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::not_null> 
    {
    using related_type = map_db_type<dbtype, 0, nullable_e::not_null>;
    using type = std::array<typename related_type::type,N>;
    
    static constexpr basic_fixed_string db_string {
      stralgo::concat_fixed_string(
        related_type::db_string,
        basic_fixed_string("("),
        strconv::integral_to_string<N>(),
        basic_fixed_string(")")
      )
      
    };
    };
}
