#pragma once

#include <stralgo/basic_fixed_string.h>

namespace cxxsql::detail
{
  using stralgo::basic_fixed_string;
  using stralgo::fixed_string;
  
  template<typename tag, unsigned N>
  struct identity_name
    {
    fixed_string<N> value_;
    
    constexpr auto value() const noexcept { return value_; }
    
    constexpr identity_name(const char (&v)[N+1]) noexcept : value_{v}
      {}
      
    constexpr identity_name( fixed_string<N> v ) noexcept : value_{v}
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
