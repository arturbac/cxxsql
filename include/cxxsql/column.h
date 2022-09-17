#pragma once

#include "detail/identity_name.h"
#include "db_data_types.h"
#include "detail/common.h"

namespace cxxsql
{
  struct column_name_tag {};
  
  template<unsigned N>
  struct column_name : public detail::identity_name<column_name_tag,N> 
    {
    column_name() noexcept = default;
    using detail::identity_name<column_name_tag,N>::identity_name;
    };

  template< unsigned N>
  column_name(char const (&str)[N])->column_name<N-1>;
  
  namespace detail
    {
    struct constraint_name_tag {};
    
    template<unsigned N>
    struct constraint_name : public detail::identity_name<constraint_name_tag,N> 
      {
      using detail::identity_name<constraint_name_tag,N>::identity_name;
      };

    template< unsigned N>
    constraint_name(char const (&str)[N])->constraint_name<N-1>;
    }
    
  template<detail::basic_fixed_string v, detail::constraint_name nm = detail::constraint_name{""}>
  struct constraint
    {
    static consteval auto value() noexcept { return v; }
    static consteval auto name() noexcept { return nm; }
    };
    
  namespace constraints
    {
    using null = constraint<"NULL">;
    using not_null = constraint<"NOT NULL">;
    using primary_key = constraint<"PRIMARY KEY">;
    using unique = constraint<"UNIQUE">;
    }
  
  namespace detail
    {
    struct contraint_list_tag;
    }
  namespace concepts
    {
    template<typename T>
    concept constraint_list = 
    requires
      {
      typename T::tag;
      requires std::same_as<typename T::tag, detail::contraint_list_tag>;
      };
    }
    
  template<typename ...Members>
//  TODO constraint name unique or empty name   requires concepts::must_be_unique_column_name<Members...>
  struct constraints_t : public Members ...
    {
    using tag = detail::contraint_list_tag;
    using first_member_t = detail::subclass_member_t<0,Members...>;
    };
    
  namespace detail
    {
    
    /// iterate over constraints and find for specified constraint
    template<typename constraint, typename MemberEnumerator>
    constexpr auto find_column_constraint() noexcept
      {
      if constexpr( !std::is_same_v<void,MemberEnumerator>)
        {
        using column_constraint = typename MemberEnumerator::member_type;
        if constexpr( constraint::value() == column_constraint::value() )
          return true;
        else
          {
          using next_enumerator = typename MemberEnumerator::next_member_t;
          return find_column_constraint<constraint,next_enumerator>();
          }
        }
      else
        return false;
      }

    template<concepts::constraint_list cstrs, typename cstr>
    consteval bool has_constraint() noexcept
      {
      return find_column_constraint<cstr, typename cstrs::first_member_t>();
      }
    }
    
  namespace concepts
    {
    template<typename cstrs>
    concept unique_nullability = requires 
      {
      requires concepts::constraint_list<cstrs>;
      requires detail::has_constraint<cstrs,constraints::not_null>() !=  detail::has_constraint<cstrs,constraints::null>();
      requires detail::has_constraint<cstrs,constraints::not_null>() ||  detail::has_constraint<cstrs,constraints::null>();
      };
    }
    
  namespace detail
    {
    template<concepts::unique_nullability cstrs>
    consteval detail::nullable_e null_constraint() noexcept
      {
      constexpr bool has_not_null { has_constraint<cstrs,constraints::not_null>() };
      return has_not_null ? detail::nullable_e::not_null : detail::nullable_e::null;
      }
    }
  //https://www.sqlite.org/lang_createtable.html
  
  //https://www.sqlite.org/syntax/column-def.html
  //https://www.postgresql.org/docs/current/datatype.html
  //column_name data_type [ COMPRESSION compression_method ] [ COLLATE collation ] [ column_constraint [ ... ] ]
  
  // where column_constraint is:
  // 
  // [ CONSTRAINT constraint_name ]
  // { NOT NULL |
  //   NULL |
  //   CHECK ( expression ) [ NO INHERIT ] |
  //   DEFAULT default_expr |
  //   GENERATED ALWAYS AS ( generation_expr ) STORED |
  //   GENERATED { ALWAYS | BY DEFAULT } AS IDENTITY [ ( sequence_options ) ] |
  //   UNIQUE index_parameters |
  //   PRIMARY KEY index_parameters |
  //   REFERENCES reftable [ ( refcolumn ) ] [ MATCH FULL | MATCH PARTIAL | MATCH SIMPLE ]
  //     [ ON DELETE referential_action ] [ ON UPDATE referential_action ] }
  // [ DEFERRABLE | NOT DEFERRABLE ] [ INITIALLY DEFERRED | INITIALLY IMMEDIATE ]
  namespace concepts
    {
    template<typename cstrs>
    concept valid_column_constraints = requires 
      {
      requires concepts::constraint_list<cstrs>;
      requires concepts::unique_nullability<cstrs>;
      };
    }
  template<column_name nm,
           typename dbtype,
           concepts::valid_column_constraints cstrs = constraints_t<constraints::not_null>
           >
  struct column_t
    {
    using type = column_t<nm,dbtype,cstrs>;
    using name_type = decltype(nm);
    static constexpr detail::nullable_e nullable =  detail::null_constraint<cstrs>();
    using db_type = detail::map_db_type<dbtype::underlying_db_type(), dbtype::size(), nullable>;
    using value_type = typename db_type::type;
    using constraints = cstrs;
    
    
    static consteval auto name() noexcept { return nm; }
    };
}
