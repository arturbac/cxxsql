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
    requires concepts::must_be_unqiue_column_name<Members...>
  struct table_t : public table_elements_t<Members ...>
    {
    static constexpr auto name() noexcept { return str; }
    };
    
  namespace detail
    {
    struct sql_command_tag {};
    }
    
  template<unsigned N>
  struct sql_command_text_t : public detail::identity_name<detail::sql_command_tag,N> 
    {
    using detail::identity_name<detail::sql_command_tag,N>::identity_name;
    };
    
  template<unsigned N>
  sql_command_text_t(char const (&str)[N])->sql_command_text_t<N>;
  
  template<unsigned N>
  sql_command_text_t(detail::fixed_string<N> v)->sql_command_text_t<N>;
  
  namespace detail
    {
    template<typename MemberEnumerator>
    constexpr auto expand_table_member() noexcept
      {
      using stralgo::fs;
      using column_type = typename MemberEnumerator::member_type;
      constexpr bool last_elem = std::is_same_v<typename MemberEnumerator::next_member_t,void>;
      return stralgo::concat_fixed_string( fs("\t\""), column_type::name().value(), fs("\" "),
                                          column_type::db_type::db_string,
                                          stralgo::cond_str<detail::nullable_e::not_null == column_type::nullable>(
                                            fs(" not_null"), fs(" null")),
                                          stralgo::cond_str<!last_elem>(fs(",\n")),
                                          expand_table_member<typename MemberEnumerator::next_member_t>() );
      }
      
    template<>
    constexpr auto expand_table_member<void>() noexcept
      { return detail::fixed_string<0>{}; }
  
    }
  template<typename table>
  consteval auto create_table_statement() noexcept
    {
    using stralgo::fs;
    return sql_command_text_t{
       stralgo::concat_fixed_string(fs("CREATE TABLE "),
                               table::name().value(),
                               fs(" (\n"),
                               detail::expand_table_member<typename table::first_member_t>(),
                               fs("\n\t)")
                               )
        };
    }
}
