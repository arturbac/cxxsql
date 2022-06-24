#pragma once

#include "detail/common.h"
#include "column.h"
#include <tuple>


namespace cxxsql
{

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
        && must_be_unqiue_column_name<NextMember, Members...>();
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
  
  struct table_name_tag {};
  
  template<unsigned N>
  struct table_name_t : public detail::identity_name<table_name_tag,N> 
    {
    using detail::identity_name<table_name_tag,N>::identity_name;
    };

  template< unsigned N>
  table_name_t(char const (&str)[N])->table_name_t<N-1>;
  
  template<table_name_t str, typename ...Members>
  requires concepts::must_be_unqiue_column_name<Members...>
  struct table_t : public columns_t<Members ...>
    {
    static constexpr auto name() noexcept { return str; }
    };
}
