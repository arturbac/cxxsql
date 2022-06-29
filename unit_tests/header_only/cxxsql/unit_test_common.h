#pragma once

#include <cxxsql/api.h>

template<typename value_type>
requires std::same_as<value_type,bool>
bool constexpr CONSTEXPR_TEST( value_type a ) {if(!(a)) throw; return true; }


