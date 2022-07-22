#pragma once

#include <system_error>

namespace cxxsql::pgsql::detail
{

  enum struct error_code_common : uint32_t {
      empty,
      ok,
      bad,
      started,  //Waiting for connection to be made.
      made,			/* connection ok; waiting to send.     */
      awaiting_response,	/* waiting for a response from the
                      * postmaster.        */
      auth_ok,			/* received authentication; waiting for
                    * backend startup. */
      setenv,			/* this state is no longer used. */
      ssl_startup,		/* negotiating ssl. */
      needed,			/* internal state: connect() needed */
      check_writable,	/* checking if session is read-write. */
      consume,			/* consuming any extra messages. */
      gss_startup,		/* negotiating gssapi. */
      check_target,	/* checking target server properties. */
      check_standby	/* checking if server is in standby mode. */
  };

  std::error_category const & pgsql_category() noexcept;

  class error_condition final : public std::error_condition
    {
    std::string description_;

  public:
    error_condition() noexcept = default;

    explicit inline error_condition( error_code_common cond ) noexcept
        : std::error_condition( static_cast<int>(cond), pgsql_category() )
      {}

    template<typename desc_type>
    inline error_condition( error_code_common cond, desc_type && description ) noexcept
        : std::error_condition( static_cast<int>(cond), pgsql_category() )
        , description_{ std::forward<desc_type>(description) }
      {}

    std::string const & description() const noexcept { return description_; }
    };
}
template<>
struct std::is_error_condition_enum<cxxsql::pgsql::detail::error_code_common>
  : public std::true_type { };

namespace cxxsql::pgsql::detail
{
  template<typename ...Args>
  inline error_condition
  make_error_condition(Args && ... args ) noexcept
    { return error_condition(std::forward<Args>(args)...); }
}
