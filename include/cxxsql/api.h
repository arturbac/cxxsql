#pragma once

#include "detail/basic_fixed_string.h"
#include <string>
#include <array>
#include <cstdint>
#include <vector>
#include <tuple>
#include <optional>

namespace cxxsql
{
  using detail::basic_fixed_string;
  using detail::fixed_string;
  
  enum struct db_type_e : uint8_t
    {
    int16, int32, int64, char8, varchar, binary, varbinary
    };
    
  enum struct nullable_e : bool
    {
    not_null, null
    };
    
  namespace detail
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
  
  namespace db_type
    {
    template<unsigned N = 0>
    using char8 = detail::db_type<db_type_e::char8, N>;
    
    template<unsigned N = 0>
    using int16 = detail::db_type<db_type_e::int16, N>;
    
    template<unsigned N = 0>
    using int32 = detail::db_type<db_type_e::int32, N>;
    
    template<unsigned N = 0>
    using int64 = detail::db_type<db_type_e::int64, N>;
    
    using varchar = detail::db_type<db_type_e::varchar>;
    
    template<unsigned N = 1>
    requires requires { N != 0; }
    using binary = detail::db_type<db_type_e::binary, N>;
    
    using varbinary = detail::db_type<db_type_e::varbinary>;
    }
  
  template<unsigned N>
  struct column_name
    {
    fixed_string<N> value_;
    constexpr column_name(const char (&v)[N+1]) : value_{v}
      {}
    constexpr auto operator<=>( column_name<N> const &) const noexcept = default;
    };
    
  template< unsigned N>
  column_name(char const (&str)[N])->column_name<N-1>;
  
  template<column_name name, typename dbtype, nullable_e nullable = nullable_e::not_null>
  struct column_t
    {
    using type = column_t<name,dbtype,nullable>;
    using value_type = detail::map_db_type_t<dbtype::underlaying_db_type(), dbtype::size(), nullable>;
    };

  template<typename ...Members>
  struct row_decl_t : public Members ...
    {
    using record_type = std::tuple<typename Members::value_type ...>;
    };
  
  template<basic_fixed_string str, typename ...Members>
  struct table_decl_t : public row_decl_t<Members ...>
    {
    static constexpr auto name() noexcept { return str; }
    };
}
