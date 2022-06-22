#pragma once

#include <string_view>
#include <algorithm>

namespace cxxsql::detail
{
  template <typename CharType, unsigned N>
  struct basic_fixed_string
    {
    using char_type = CharType;

    char_type m_data[N+1];
    
    constexpr basic_fixed_string(const char_type (&foo)[N+1])
      { std::copy_n(foo, N+1, m_data); }

    constexpr std::basic_string_view<char_type> value() const noexcept 
      { return {&m_data[0], N}; }
    
    constexpr auto operator<=>( basic_fixed_string<char_type,N> const &) const noexcept = default;
    };

  template <typename char_type, unsigned N>
  basic_fixed_string(char_type const (&str)[N])->basic_fixed_string<char_type, N-1>;

  template <unsigned N>
  using fixed_string = basic_fixed_string<char, N>;
}
