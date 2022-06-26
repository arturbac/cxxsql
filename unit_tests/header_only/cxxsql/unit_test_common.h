#pragma once

#include <cxxsql/api.h>

namespace fixedmath
{

  template<typename expected, typename input>
  constexpr bool test_resulting_type( input ) { return std::is_same<expected,input>::value; }

}

