#include <cxxsql/pqsql/connection.h>
#include <cxxsql/pqsql/detail/error_condition.h>
#include <cxxsql/detail/pragmas.h>
#include <cxxsql/detail/core_db_functionality.h>
#include <coll/small_vector.h>
#include <algorithm>
#include <string>
#include <numeric>
#include <libpq-fe.h>
#include <utility>

namespace cxxsql::pgsql::detail
{
  struct pgsql_category_t final : public std::error_category
    {
    char const * name() const noexcept override { return "pgsql_category"; }
    std::string message(int) const override { return "postgresql error codes"; }
    ~pgsql_category_t() override;
    };
  pgsql_category_t::~pgsql_category_t(){}

PRAGMA_CLANG_WARNING_PUSH_OFF(-Wexit-time-destructors)
PRAGMA_CLANG_WARNING_OFF(-Wglobal-constructors)
  static pgsql_category_t pgsql_category_{};
PRAGMA_CLANG_WARNING_POP

  std::error_category const & pgsql_category() noexcept
    { return pgsql_category_; }
}

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
               std::launder(reinterpret_cast<connection_t::pimpl_t *>(resource_data_))); }

static connection_t::pimpl_t const *
connection_data(std::byte const * resource_data_ ) noexcept
   { return std::assume_aligned<alignof(connection_t::pimpl_t)>(
               std::launder(reinterpret_cast<connection_t::pimpl_t const *>(resource_data_))); }

connection_t::pimpl_t & connection_t::pimpl() noexcept
  {
  return *connection_data(resource_data_);
  }
connection_t::pimpl_t const & connection_t::pimpl() const noexcept
  {
  return *connection_data(resource_data_);
  }

void connection_t::free_resources() noexcept
  {
  auto & data{ pimpl() };
  if(data.con )
    {
//     printf("free %lX\n", reinterpret_cast<size_t>(data.con) );
    PQfinish(data.con);
    data.con = nullptr;
    }
  backend_ = backends_e::destroyed;
  }
  
connection_t::connection_t( pimpl_t const & res ) noexcept 
    : connection_base<connection_t>{ backends_e::pgsql }
  {
  auto & data = pimpl();
  data = res;
//   printf("construct [%lX].con=%lX\n", reinterpret_cast<size_t>(&data), reinterpret_cast<size_t>(data.con) );
  }
  
connection_t::operator bool() const noexcept
  {
  auto const & data{ pimpl() };
  if(data.con)
    {
    ConnStatusType const status{ PQstatus(data.con) };
    return CONNECTION_OK == status;
    }
  return false;
  }

detail::error_condition connection_t::status() const noexcept
  {
  auto const & data{ pimpl() };
  if(data.con)
    {
    ConnStatusType const s{ PQstatus(data.con) };
    if(CONNECTION_BAD == s)
      {
      auto message {PQerrorMessage(data.con) };
      return detail::make_error_condition(pgsql::detail::error_code_common::bad, message );
      }
    else
      return detail::make_error_condition( static_cast<pgsql::detail::error_code_common>(static_cast<int>(s)+1) );
    }
  return detail::make_error_condition( pgsql::detail::error_code_common::empty );
  }

connection_t::connection_t( connection_t && rh ) noexcept
    : cxxsql::detail::connection_base<connection_t>( std::move(rh) )
  {
  pimpl() = std::exchange(rh.pimpl(), {});
  backend_ = std::exchange( rh.backend_, cxxsql::detail::backends_e::destroyed);
  }

connection_t & connection_t::operator =( connection_t && rh ) noexcept
  {
  pimpl() = std::exchange(rh.pimpl(), {});
  backend_ = std::exchange( rh.backend_, cxxsql::detail::backends_e::destroyed);
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
    coll::small_vector<char,uint32_t,1024u> keys_values(static_cast<uint32_t>(total_chars));
    coll::small_vector<char const *,uint8_t,16> keys_values_ptrs;
    keys_values_ptrs.resize(static_cast<uint8_t>(args_count<<1u));
    
    auto const it_beg_ok{ std::begin(keys_values_ptrs) };
    auto it_ok{ it_beg_ok };
    auto const it_beg_ov{ std::next(std::begin(keys_values_ptrs), static_cast<ptrdiff_t>(args_count + 1u) ) };
    auto it_ov{ it_beg_ov };
    
    auto itk{ keys_values.begin() };
    for( auto const & kvp : params )
      {
      auto itv{ std::copy(std::begin(kvp.first), std::end(kvp.first), itk) };
      *itv++ = {};
      auto nxt{ std::copy(std::begin(kvp.second), std::end(kvp.second), itv) };
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
    using pimpl_t = connection_t::pimpl_t;
//     printf("alloc %lX\n", reinterpret_cast<size_t>(pgcon) );
    return connection_t { pimpl_t{ .con=pgcon } };
    }
  return {};
  }

  std::string_view database( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQdb( data.con );
    return {};
    }

  std::string_view user( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQuser( data.con );
    return {};
    }

  std::string_view password( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQpass( data.con );
    return {};
    }

  std::string_view host( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQhost( data.con );
    return {};
    }

  std::string_view hostaddr( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQhostaddr( data.con );
    return {};
    }

  std::string_view port( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQport( data.con );
    return {};
    }

  int protocol_version( connection_t const & conn ) noexcept
    {
    auto const & data{ conn.pimpl() };
    if(data.con != nullptr)
      return PQprotocolVersion( data.con );
    return -1;
    }
}
