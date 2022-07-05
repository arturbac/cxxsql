#pragma once

#include "connection_handle.h"
#include <concepts>

namespace cxxsql::detail
{

template<typename return_type, bool is_noexcept, concepts::connection_handle maybe_handle, typename lambda>
return_type dispatch( maybe_handle & handle, lambda const & op ) noexcept(is_noexcept)
  {
  switch( handle.backend_ )
    {
    case backends_e::pgsql:
      return op( *static_cast<cxxsql::pgsql::connection_t *>(&handle) );
     case backends_e::sqlite:
//       return op( *static_cast<cxxsql::sqlite::connection_t *>(&handle) );
     case backends_e::destroyed:
       break;
    }
  if constexpr( std::is_same_v<void,return_type>)
    return;
  else
    return return_type{};
  }
}
