#pragma once

#include <cstdint>
#include <concepts>
#include "pragmas.h"

#if defined(cxxsql_EXPORTS)
  #define CXXSQL_PUBLIC_DECL ATTRIBUTE_EXPORT_DECL
#else
  #define CXXSQL_PUBLIC_DECL ATTRIBUTE_IMPORT_DECL
#endif
namespace cxxsql::detail
{
  enum struct backends_e : uint8_t { destroyed, pgsql, sqlite };
}
namespace cxxsql::pgsql
{
class connection_t;
}
namespace cxxsql::sqlite
{
class connection_t;
}
namespace cxxsql::detail
{
struct connection_handle
  {
  [[no_unique_address]]
  backends_e backend_;
  
protected:
  explicit connection_handle( backends_e backend ) noexcept 
    : backend_{backend}
    { }

  connection_handle( connection_handle const & ) = delete;
  connection_handle( connection_handle && ) noexcept = default;
  
  connection_handle & operator =( connection_handle const & ) = delete;
  connection_handle & operator =( connection_handle && ) noexcept = default;
  
public:
  inline ~connection_handle() noexcept;
  
  };
  
template<typename Super>
class connection_base : public connection_handle
  {
protected:
  Super const & This() const noexcept { return *static_cast<Super const *>(this); }
  Super & This() noexcept { return *static_cast<Super *>(this); }
  
protected:
  using connection_handle::connection_handle;
  };
  
namespace concepts
  {
  template<typename maybe_handle>
  concept connection_handle = std::same_as<connection_handle, std::remove_cv_t<maybe_handle>>;
  }
  
template<typename return_type, bool is_noexcept, concepts::connection_handle maybe_handle, typename lambda>
return_type dispatch( maybe_handle & handle, lambda const & op ) noexcept(is_noexcept);

inline connection_handle::~connection_handle() noexcept
  {
  dispatch<void,true>(*this,[](auto & promoted)
                                { promoted.free_resources(); });
  }
}
