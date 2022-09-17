#pragma once

#include "detail/identity_name.h"

namespace cxxsql
{
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
  
}
