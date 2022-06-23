#pragma once

#include "detail/common.h"
#include "detail/basic_fixed_string.h"
#include "detail/db_type.h"
#include <tuple>


namespace cxxsql
{
  namespace property
    {
    inline constexpr auto null = detail::nullable_e::null;
    inline constexpr auto not_null = detail::nullable_e::not_null;

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
  
  template<unsigned N>
  struct column_name
    {
    detail::fixed_string<N> value_;
    
    constexpr column_name(const char (&v)[N+1]) : value_{v}
      {}
      
    template<unsigned M>
    constexpr bool operator ==( column_name<M> const & r) const noexcept 
      { return value_ == r.value_; }
      
    template<unsigned M>
    constexpr auto operator <=>( column_name<M> const & r) const noexcept 
      { return value_ <=> r.value_; }
      
    constexpr std::basic_string_view<char> view() const noexcept 
      { return value_.value(); }
    };
    
  template< unsigned N>
  column_name(char const (&str)[N])->column_name<N-1>;
  
  template<column_name nm, typename dbtype, detail::nullable_e nullable = detail::nullable_e::not_null>
  struct column_t
    {
    static consteval auto name() noexcept { return nm; }
    using type = column_t<nm,dbtype,nullable>;
    using value_type = detail::map_db_type_t<dbtype::underlaying_db_type(), dbtype::size(), nullable>;
    };

  namespace detail
  {
    template<typename Member>
    consteval bool must_be_unqiue_column_name() noexcept { return true; }

    template<typename Member, typename NextMember, typename ...Members>
    consteval bool must_be_unqiue_column_name() noexcept
      {
      //verify this member name is unique across all members
      return Member::name() != NextMember::name() && must_be_unqiue_column_name<Member, Members...>()
        //verify next member with other members
        && must_be_unqiue_column_name<NextMember, Members...>()
        ;
      }
  }
  namespace concepts
    {
    template<typename ...Members>
    concept must_be_unqiue_column_name = detail::must_be_unqiue_column_name<Members...>();
    }
    
  template<typename ...Members>
  requires concepts::must_be_unqiue_column_name<Members...>
  struct columns_t : public Members ...
    {
    using record_type = std::tuple<typename Members::value_type ...>;
    };
  
  template<detail::basic_fixed_string str, typename ...Members>
  requires concepts::must_be_unqiue_column_name<Members...>
  struct table_t : public columns_t<Members ...>
    {
    static constexpr auto name() noexcept { return str; }
    };
}
