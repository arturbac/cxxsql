#pragma once

#include <cxxsql/detail/connection_handle.h>

#include <string_view>
#include <span>
#include <memory>

namespace cxxsql::pgsql
{
class connection_t final : public cxxsql::detail::connection_base<connection_t>
  {
public:
  friend class connection_base<connection_t>;
  friend struct connection_handle;
  using backends_e = cxxsql::detail::backends_e;
  struct pimpl_t;
  
private:
  alignas(void *)
  std::byte resource_data_[sizeof(void *)];
  
public:
  
  [[nodiscard]]
  explicit operator bool() const noexcept;
  
  connection_t() noexcept 
    : connection_base<connection_t>{ backends_e::destroyed }
  {}
  
  explicit connection_t( pimpl_t const & res ) noexcept;
  
  connection_t( connection_t const & ) = delete;
  connection_t( connection_t && ) noexcept;
  
  connection_t & operator =( connection_t const & ) = delete;
  connection_t & operator =( connection_t && ) noexcept;
  
  ~connection_t() noexcept { free_resources(); }

  void close() noexcept { free_resources(); }

protected:
  void free_resources() noexcept;

  };
  
  
  using open_params_type = std::span<std::pair<std::string_view, std::string_view>>;

  connection_t open( open_params_type params ) noexcept;
  inline void close(connection_t & conn) noexcept { conn.close(); }
}
