#pragma once

#include <cxxsql/detail/common.h>


namespace cxxsql::pgsql::detail
{
  enum struct db_type_e : uint8_t
    {
    smallserial,
    serial,
    bigserial,
    text,
    bytea,
    boolean,
    cidr, //IPv4 or IPv6 network address
    date,
    double_precision,
    inet, //IPv4 or IPv6 host address
    json,
    jsonb,
    uuid,
    xml
    };
}
namespace cxxsql::detail
{
  template<>
  struct backend_decl_t<backend_e::pqsql,pgsql::detail::db_type_e>
    {
    static constexpr backend_e backend_enum = backend_e::pqsql;
    using db_type_enum = pgsql::detail::db_type_e;
    };
}
