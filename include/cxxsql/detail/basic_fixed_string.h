#pragma once

#include <string_view>
#include <algorithm>
#include <array>
#include <concepts>

namespace cxxsql::concepts
{
  template<typename T, typename ... U>
  concept is_any_of = (std::same_as<T, U> || ...);

  template<typename maybe_char_type>
  concept is_char_type = is_any_of<maybe_char_type,char,wchar_t,char16_t,char32_t,char8_t>;
}
namespace cxxsql::detail
{
  template <concepts::is_char_type CharType, unsigned N>
  struct basic_fixed_string
    {
    using char_type = CharType;

    char_type data_[N+1]{};
    
    constexpr unsigned size() const noexcept { return N; }
    constexpr char_type const * begin() const noexcept { return &data_[0]; }
    constexpr char_type * data() noexcept { return &data_[0]; }
    constexpr char_type const * end() const noexcept { return &data_[N]; }
    
    constexpr basic_fixed_string() noexcept : data_{}{}
    
    constexpr basic_fixed_string(const char_type (&foo)[N+1]) noexcept
      { std::copy_n(foo, N+1, data_); }
      
    constexpr std::basic_string_view<char_type> view() const noexcept 
      { return {&data_[0], N}; }
    
    constexpr auto operator<=>( basic_fixed_string<char_type,N> const &) const noexcept = default;
    
    template<unsigned M>
    constexpr auto operator == ( basic_fixed_string<char_type,M> const & r) const noexcept
      { return N == M && view() == r.view(); }
    };

  template <concepts::is_char_type char_type, unsigned N>
  basic_fixed_string(char_type const (&str)[N])->basic_fixed_string<char_type, N-1>;

  template <unsigned N>
  using fixed_string = basic_fixed_string<char, N>;
  
  template <concepts::is_char_type char_type, unsigned N>
  consteval unsigned calculate_size(basic_fixed_string<char_type,N> ) noexcept { return N; }
  
  template <concepts::is_char_type char_type, unsigned N, typename ... U>
  consteval unsigned calculate_size(basic_fixed_string<char_type,N>, U ...u) noexcept 
    { return N + calculate_size(u...); }
  
  template <concepts::is_char_type char_type, unsigned N, unsigned M>
  consteval auto concat_fixed_string( basic_fixed_string<char_type,N> l, basic_fixed_string<char_type,M> r) noexcept
    {
    basic_fixed_string<char_type, N + M> result;
    auto it { std::copy( l.begin(), l.end(), result.data() ) };
    it = std::copy( r.begin(), r.end(), it );
    *it= {};
    return result;
    }

  template <concepts::is_char_type char_type, unsigned N, unsigned M, typename ... U>
  consteval auto concat_fixed_string( basic_fixed_string<char_type,N> l, basic_fixed_string<char_type,M> r, U ...u) noexcept
    {
    return concat_fixed_string( l, concat_fixed_string(r,u...));
    }
    
  template <bool cond, concepts::is_char_type char_type, unsigned N, unsigned M>
  consteval auto cond_str( basic_fixed_string<char_type,N> l, basic_fixed_string<char_type,M> r )
    {
    if constexpr (cond)
      return l;
    else
      return r;
    }
    
    
  template <concepts::is_char_type char_type, unsigned N>
  consteval auto fs(char_type const (&str)[N])
    { return detail::basic_fixed_string<char_type, N-1>(str); }
}
