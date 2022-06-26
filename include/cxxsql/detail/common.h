#pragma once

#include <cstdint>

namespace cxxsql::detail
{
  
  enum struct db_type_e : uint8_t
    {
    int16, int32, int64, char8, varchar, binary, varbinary
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
