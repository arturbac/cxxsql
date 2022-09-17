#pragma once

#include "detail/common.h"
#include "column.h"
#include <tuple>

namespace cxxsql
{
  // https://www.postgresql.org/docs/14/sql-createtable.html
  // options TEMPORARY | UNLOGGED
  namespace detail
    {
    struct table_option_tag {};
    }
  
  template< unsigned N>
  struct table_option_t : public detail::identity_name<detail::table_option_tag,N>
    {
    using detail::identity_name<detail::table_option_tag,N>::identity_name;
    };
    
  template< unsigned N>
  table_option_t(char const (&str)[N])->table_option_t<N-1>;
  
  namespace detail
  {
      
    template<typename Member>
    consteval bool must_be_unique_column_name() noexcept { return true; }

    template<typename Member, typename NextMember, typename ...Members>
    consteval bool must_be_unique_column_name() noexcept
      {
      //verify this member name is unique across all members
      return Member::name() != NextMember::name() && must_be_unique_column_name<Member, Members...>()
        //verify next member with other members
        && must_be_unique_column_name<NextMember, Members...>();
      }
  }
  namespace concepts
    {
    template<typename ...Members>
    concept must_be_unique_column_name = detail::must_be_unique_column_name<Members...>();
    }
    
  template<typename ...Members>
    requires concepts::must_be_unique_column_name<Members...>
  struct table_elements_t : public Members ...
    {
    using record_type = std::tuple<typename Members::value_type ...>;
    using first_member_t = detail::subclass_member_t<0,Members...>;
    };
  
  namespace detail
    {
    struct table_name_tag {};
    }
  
  template<unsigned N>
  struct table_name_t : public detail::identity_name<detail::table_name_tag,N> 
    {
    using detail::identity_name<detail::table_name_tag,N>::identity_name;
    };

  template< unsigned N>
  table_name_t(char const (&str)[N])->table_name_t<N-1>;
  
  template<table_name_t str, typename ...Members>
    requires concepts::must_be_unique_column_name<Members...>
  struct table_t : public table_elements_t<Members ...>
    {
    static constexpr auto name() noexcept { return str; }
    
    using column_names_t = std::tuple<typename Members::name_type ...>;
    
    static constexpr column_names_t column_names() noexcept 
      { return column_names_t{Members::name().value() ...};}
    };
    
  template<typename table_type>
  consteval auto column_names() noexcept 
    {
    return table_type::column_names();
    }
  
}
