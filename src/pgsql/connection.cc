#include <cxxsql/pqsql/connection.h>
#include <cxxsql/detail/core_db_functionality.h>
#include <vector>
#include <algorithm>
#include <string>
#include <numeric>
#include <libpq-fe.h>
#include <utility>

namespace cxxsql::pgsql
{
  
struct connection_t::pimpl_t
{
  PGconn * con;
};

[[nodiscard]]
static connection_t::pimpl_t *
connection_data(std::byte * resource_data_ ) noexcept
   { return std::assume_aligned<alignof(connection_t::pimpl_t)>(
               std::launder(reinterpret_cast<connection_t::pimpl_t *>(&resource_data_))); }
   
void connection_t::free_resources() noexcept
  {
  if(resource_ )
    {
    PQfinish(resource_->con);
    resource_.reset();
    }
  backend_ = backends_e::destroyed;
  }
  
connection_t::connection_t( pimpl_t const & res ) noexcept 
    : connection_base<connection_t>{ backends_e::pgsql }
  {
  *connection_data(resource_data_) = res;
  }
  
connection_t::connection_t( connection_t && rh ) noexcept
    : cxxsql::detail::connection_base<connection_t>( std::move(rh) )
  {
  *connection_data(resource_data_) = std::exchange(*connection_data(rh.resource_data_), {});
  backend_ = std::exchange( rh.backend_,detail::backends_e::destroyed);
  }

connection_t & connection_t::operator =( connection_t && rh ) noexcept
  {
  *connection_data(resource_data_) = std::exchange(*connection_data(rh.resource_data_), {});
  backend_ = std::exchange( rh.backend_,detail::backends_e::destroyed);
  return *this;
  }
  
connection_t open( open_params_type params ) noexcept
  {
  if( !params.empty() )
    {
    // count number of chars + null terminations
    auto const args_count{params.size()+1};
    std::size_t total_chars{
        (params.size()<<1) +
        std::accumulate(std::begin(params), std::end(params), size_t{},
                    [](std::size_t init, auto const & kvp) -> std::size_t
                    { return init + kvp.first.size() + kvp.second.size(); }) };

    // render strings in table
    std::vector<char> keys_values(total_chars);
    std::vector<char const *> keys_values_ptrs;
    keys_values_ptrs.resize(args_count<<1);
    
    auto const it_beg_ok{ std::begin(keys_values_ptrs) };
    auto it_ok{ it_beg_ok };
    auto const it_beg_ov{ std::next(std::begin(keys_values_ptrs), static_cast<ptrdiff_t>(args_count + 1u) ) };
    auto it_ov{ it_beg_ov };
    
    auto itk{ keys_values.begin() };
    for( auto const & kvp : params )
      {
      auto itv{ std::copy(std::begin(kvp.first), std::end(kvp.first), itk) };
      *itv++ = {};
      auto nxt{ std::copy(std::begin(kvp.first), std::end(kvp.first), itv) };
      *nxt++ = {};
      *it_ok++ = &*itk;
      *it_ov++ = &*itv;
      itk = nxt;
      }
    //https://www.postgresql.org/docs/current/libpq-connect.html#LIBPQ-PQCONNECTDBPARAMS
    constexpr int expand_dbname = 0;
    char const * const * const keywords{ &*it_beg_ok };
    char const * const * const values{ &*it_beg_ov };
    PGconn * pgcon{ PQconnectdbParams(keywords, values, expand_dbname) };
    ConnStatusType const status{ PQstatus(pgcon) };
    if(CONNECTION_OK == status )
      {
      using pimpl_t = connection_t::pimpl_t;
      connection_t conn{ pimpl_t{ .con=pgcon } };
      return conn;
      }
    }
  return {};
  }
}
