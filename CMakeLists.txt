cmake_minimum_required(VERSION 3.20)

project(cxx20sql)

enable_testing()

set(CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF )

include(FetchContent)
FetchContent_Declare(
  small_vectors
  GIT_REPOSITORY https://github.com/arturbac/small_vectors.git
  GIT_TAG        master
)
FetchContent_Declare(
  ut
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(small_vectors)
FetchContent_MakeAvailable(ut)

find_package(PostgreSQL )
find_package(SQLite3)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options( -Wall -Werror )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options( 
      -Weverything 
      -Werror 
      -Wno-pre-c++17-compat
      -Wno-c++98-compat
      -Wno-pre-c++14-compat
      -Wno-c++20-compat
      -Wno-pre-c++20-compat-pedantic
      -Wno-unused-parameter
      -Wno-padded
      -Wno-unused-command-line-argument
      )
      
    add_compile_options( -fuse-ld=lld )
    add_link_options( -fuse-ld=lld )
    if( CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options( -flto=thin )
    endif()
    
    
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if( CMAKE_BUILD_TYPE STREQUAL "Release")
      add_compile_options( -fvisibility=hidden
              -fvisibility-inlines-hidden )
  else()
      add_compile_options(-fvisibility=default )
  endif()
endif()

add_library(cxxsql)
target_sources(cxxsql 
  PRIVATE
    src/pgsql/connection.cc 
)

target_include_directories(cxxsql PUBLIC include )
add_library(cxxsql::cxxsql ALIAS cxxsql)

target_link_libraries( cxxsql
  PRIVATE 
    small_vectors::small_vectors
    SQLite::SQLite3
    PostgreSQL::PostgreSQL
  )
  
add_executable(cxxsqlm main.cpp)
target_link_libraries( cxxsqlm 
  PRIVATE cxxsql::cxxsql 
    SQLite::SQLite3
    PostgreSQL::PostgreSQL
  )

add_subdirectory(unit_tests)

