#pragma once

#include "basic_fixed_string.h"

namespace cxxsql::detail
{
  template<typename tag, unsigned N>
  struct identity_name
    {
    detail::fixed_string<N> value_;
    
    constexpr identity_name(const char (&v)[N+1]) : value_{v}
      {}
      
    template<unsigned M>
    constexpr bool operator ==( identity_name<tag, M> const & r) const noexcept 
      { return value_ == r.value_; }
      
    template<unsigned M>
    constexpr auto operator <=>( identity_name<tag, M> const & r) const noexcept 
      { return value_ <=> r.value_; }
      
    constexpr std::basic_string_view<char> view() const noexcept 
      { return value_.value(); }
    };
}
