#pragma once

#include "detail/common.h"
#include "column.h"
#include "sql_command.h"
#include "table.h"



namespace cxxsql
{

  // create_table_statement impl
  
  namespace detail
    {
    template<typename MemberEnumerator>
    constexpr auto expand_column_constraints() noexcept
      {
      using stralgo::fs;
      using column_constraint = typename MemberEnumerator::member_type;
      using next_column_constraint = typename MemberEnumerator::next_member_t;
      return stralgo::concat_fixed_string(
                      fs(" "),
                      column_constraint::value(),
                      expand_column_constraints<next_column_constraint>() 
                      );
      }
    template<>
    constexpr auto expand_column_constraints<void>() noexcept
      { return detail::fixed_string<0>{}; }
      
    template<typename MemberEnumerator>
    constexpr auto expand_table_member() noexcept
      {
      using stralgo::fs;
      using column_type = typename MemberEnumerator::member_type;
      using next_column_type = typename MemberEnumerator::next_member_t;
      constexpr bool last_elem = std::is_same_v<next_column_type,void>;
      return stralgo::concat_fixed_string( fs("\t\""), column_type::name().value(), fs("\" "),
                                          column_type::db_type::db_string,
                                          expand_column_constraints<typename column_type::constraints::first_member_t>(),
                                          stralgo::cond_str<!last_elem>(fs(",\n")),
                                          expand_table_member<next_column_type>() );
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
