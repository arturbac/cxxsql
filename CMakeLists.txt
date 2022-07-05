cmake_minimum_required(VERSION 3.20)

project(cxx20sql)

enable_testing()

set(CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF )

find_package(ut REQUIRED ut)
#find_package(small_vectors  REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

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

add_executable(cxxsqlm main.cpp)
target_link_libraries( cxxsqlm 
  PRIVATE cxxsql::cxxsql 
    unofficial::sqlite3::sqlite3
    PostgreSQL::PostgreSQL
  )

add_subdirectory(unit_tests)

