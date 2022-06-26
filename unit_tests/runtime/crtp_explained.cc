#include <boost/ut.hpp>
#include <cxxsql/api.h>
#include <memory>
#include <iostream>

enum struct backends_e : uint8_t { destroyed, pgsql, sqlite };

namespace cxxsql::pgsql
{
struct connection_t;
}
namespace cxxsql::sqlite
{
struct connection_t;
}
namespace cxxsql
{
struct connection_handle
  {
  [[no_unique_address]]
  backends_e backend_;
  
protected:
  explicit connection_handle( backends_e backend ) noexcept 
    : backend_{backend}
    { }
public:
  connection_handle( connection_handle const & ) = delete;
  connection_handle & operator =( connection_handle const & ) = delete;
  ~connection_handle() noexcept;
  
public:
  bool open( std::string_view args ) noexcept;
  };
  
template<typename Super>
struct connection_base : public connection_handle
  {
protected:
  Super const & This() const noexcept { return *static_cast<Super const *>(this); }
  Super & This() noexcept { return *static_cast<Super *>(this); }
  
protected:
  using connection_handle::connection_handle;
  
public:
  bool open( std::string_view args ) noexcept
    {
    //base common logic
    return This().open_impl(args);
    }
  };
}

namespace cxxsql::pgsql
{
struct connection_t final : public connection_base<connection_t>
  {
  friend struct connection_base<connection_t>;
  friend struct connection_handle;
private:
  
  void const * resource{};
  
public:
  connection_t() noexcept : connection_base<connection_t>{ backends_e::pgsql }
    {}
  
  ~connection_t() noexcept
    {
    free_resources();
    }
    
  void free_resources() noexcept
    {
    if(nullptr != resource )
      { 
      backend_ = backends_e::destroyed;
      resource = nullptr;
      std::cout << "pgsql resources cleared" << std::endl;
      }
    }
    
  bool open_impl(std::string_view args) noexcept
    {
    //backend specific logic
    resource = args.data();
    std::cout << "pgsql connection opened" << std::endl;
    return true;
    }
  };
}

namespace cxxsql
{
  connection_handle::~connection_handle() noexcept
    {
    switch( backend_ )
      {
      case backends_e::pgsql:
        static_cast<cxxsql::pgsql::connection_t *>(this)->free_resources();
        break;
      case backends_e::sqlite:
        break;
      case backends_e::destroyed:
        break;
      }
    }
  bool connection_handle::open( std::string_view args ) noexcept
    {
    switch( backend_ )
      {
      case backends_e::pgsql:
        return static_cast<cxxsql::pgsql::connection_t *>(this)->open(args);
      case backends_e::sqlite:
        break;
      case backends_e::destroyed:
        break;
      }
    return false;
    }
    
  //or as a function
  inline bool open(connection_handle & handle, std::string_view args ) noexcept
    {
    switch( handle.backend_ )
      {
      case backends_e::pgsql:
        return static_cast<cxxsql::pgsql::connection_t *>(&handle)->open(args);
      case backends_e::sqlite:
        break;
      case backends_e::destroyed:
        break;
      }
    return false;
    }
}

int main() {
  //type determined at runtime with small cost much lower than dynamic_cast
  {
  std::unique_ptr<cxxsql::connection_handle> ptr{ std::make_unique<cxxsql::pgsql::connection_t>()};
  ptr->open("test");
  }
  //type used directly at no cost
  {
  auto ptr{ std::make_unique<cxxsql::pgsql::connection_t>()};
  ptr->open("test");
  }
}
