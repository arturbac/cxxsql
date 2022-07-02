#pragma once

#include <cstdint>
#include <type_traits>
#include <concepts>

namespace cxxsql::concepts
{
  template<typename T, typename ... U>
  concept is_any_of = (std::same_as<T, U> || ...);
  
  template<typename T>
  concept enum_type = std::is_enum_v<T>;
}

namespace cxxsql::detail
{
  enum struct backend_e { universal, pqsql, sqlite };
	
  enum struct db_type_e : uint8_t
    {
    int16, int32, int64, char8, varchar, binary, varbinary
    };
    
  template<backend_e backend = backend_e::universal,
           cxxsql::concepts::enum_type DbTypeEnum = db_type_e>
  struct backend_decl_t
    {
    static constexpr backend_e backend_enum = backend_e::universal;
    using db_type_enum = DbTypeEnum;
    };
    
  enum struct nullable_e : bool
    {
    not_null, null
    };
    
  template<unsigned counter, typename Member, typename ...Members>
  struct subclass_member_t
    {
    using member_type = Member;
    using next_member_t = subclass_member_t<(counter+1), Members...>;
    
    static constexpr unsigned index() noexcept { return counter; }
    };
    
  template<unsigned counter, typename Member>
  struct subclass_member_t<counter,Member>
    {
    using member_type = Member;
    using next_member_t = void;
    static constexpr unsigned index() noexcept { return counter; }
    };
  template<>
  struct subclass_member_t<0,void>
    {
    using member_type = void;
    using next_member_t = void;
//     static constexpr unsigned index() noexcept { return counter; }
    };
}

