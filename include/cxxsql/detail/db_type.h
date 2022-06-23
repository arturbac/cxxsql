#pragma once

#include "common.h"
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
  struct map_db_type<db_type_e::binary, 0, nullable_e::not_null> { using type = uint8_t; };
  
  template<>
  struct map_db_type<db_type_e::char8, 0, nullable_e::not_null> { using type = char; };
  
  template<>
  struct map_db_type<db_type_e::int16, 0, nullable_e::not_null> { using type = int16_t; };
  
  template<>
  struct map_db_type<db_type_e::int32, 0, nullable_e::not_null> { using type = int32_t; };
  
  template<>
  struct map_db_type<db_type_e::int64, 0, nullable_e::not_null> { using type = int64_t; };
  
  template<>
  struct map_db_type<db_type_e::varchar, 0, nullable_e::not_null> { using type = std::string; };
  
  template<>
  struct map_db_type<db_type_e::varbinary, 0, nullable_e::not_null> { using type = std::vector<uint8_t>; };
  
  template<db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::not_null> 
    { using type = std::array<typename map_db_type<dbtype, 0, nullable_e::not_null>::type,N>; };
    
  template<db_type_e dbtype, unsigned N>
  struct map_db_type<dbtype, N, nullable_e::null> 
    { using type = std::optional<typename map_db_type<dbtype, N, nullable_e::not_null>::type>; };
  
  template<db_type_e dbtype, unsigned N, nullable_e nullable>
  using map_db_type_t = typename map_db_type<dbtype, N, nullable>::type;
}
