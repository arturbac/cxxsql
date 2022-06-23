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
}
