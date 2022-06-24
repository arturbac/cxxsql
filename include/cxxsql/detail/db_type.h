#pragma once

#include "common.h"
#include "basic_fixed_string.h"
#include <string>
#include <array>
#include <vector>
#include <optional>

namespace cxxsql::detail
{
  template<db_type_e dbtype, unsigned N = 0>
  struct db_type
    {
    static constexpr db_type_e underlaying_db_type() noexcept { return dbtype; }
    static constexpr bool is_array() noexcept { return N != 0; }
    static constexpr unsigned size() noexcept { return N; }
    };
    
  template<db_type_e dbtype, unsigned N, nullable_e nullable>
  struct map_db_type  {};
  
  template<>
  struct map_db_type<db_type_e::binary, 0, nullable_e::not_null>
    { 
    using type = uint8_t;
    static constexpr basic_fixed_string db_string {"binary"};
    };
  
  template<>
  struct map_db_type<db_type_e::char8, 0, nullable_e::not_null>
    { 
    using type = char;
    static constexpr basic_fixed_string db_string {"char"};
    };
  
  template<>
  struct map_db_type<db_type_e::int16, 0, nullable_e::not_null>
    {
    using type = int16_t;
    static constexpr basic_fixed_string db_string {"smallint"};
    };
  
  template<>
  struct map_db_type<db_type_e::int32, 0, nullable_e::not_null> 
    {
    using type = int32_t;
    static constexpr basic_fixed_string db_string {"int"};
    };
  
  template<>
  struct map_db_type<db_type_e::int64, 0, nullable_e::not_null>
    {
    using type = int64_t;
    static constexpr basic_fixed_string db_string {"bigint"};
    };
  
  template<>
  struct map_db_type<db_type_e::varchar, 0, nullable_e::not_null> 
    {
    using type = std::string;
    static constexpr basic_fixed_string db_string {"varchar"};
    };
  
  template<>
  struct map_db_type<db_type_e::varbinary, 0, nullable_e::not_null>
    {
    using type = std::vector<uint8_t>;
    static constexpr basic_fixed_string db_string {"varbinary"};
    };
  
  template<db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::not_null> 
    {
    using related_type = map_db_type<dbtype, 0, nullable_e::not_null>;
    using type = std::array<typename related_type::type,N>;
    
    static constexpr basic_fixed_string db_string {
      concat_fixed_string(
        related_type::db_string,
        basic_fixed_string("["),
        basic_fixed_string("]")
      )
      
    };
    };
    
  template<db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::null> 
    {
    using related_type = map_db_type<dbtype, N, nullable_e::not_null>;
    using type = std::optional<typename related_type::type>;
    static constexpr basic_fixed_string db_string { related_type::db_string };
    };
  
  template<db_type_e dbtype, unsigned N, nullable_e nullable>
  using map_db_type_t = typename map_db_type<dbtype, N, nullable>::type;
}
