#pragma once

#include "detail/identity_name.h"
#include "db_data_types.h"

namespace cxxsql
{
  struct column_name_tag {};
  
  template<unsigned N>
  struct column_name : public detail::identity_name<column_name_tag,N> 
    {
    using detail::identity_name<column_name_tag,N>::identity_name;
    };

  template< unsigned N>
  column_name(char const (&str)[N])->column_name<N-1>;
  
  template<column_name nm, typename dbtype, detail::nullable_e nbl = detail::nullable_e::not_null>
  struct column_t
    {
    static constexpr detail::nullable_e nullable = nbl;
    static consteval auto name() noexcept { return nm; }
    using type = column_t<nm,dbtype,nullable>;
    using db_type = detail::map_db_type<dbtype::underlaying_db_type(), dbtype::size(), nullable>;
    using value_type = typename db_type::type;
    };
}
