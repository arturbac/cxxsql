cmake_minimum_required(VERSION 3.20)

project(cxx20sql)

set(CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF )
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

add_library(cxx20sql INTERFACE)

target_include_directories(cxx20sql INTERFACE include )
add_library(cxx20sql::cxx20sql ALIAS cxx20sql)
#target_sources(cxx20sql INTERFACE
  #FILE_SET HEADERS
    #BASE_DIRS src
    #FILES include/
#)

add_executable(cxx20sqlm main.cpp)
target_link_libraries( cxx20sqlm PRIVATE cxx20sql::cxx20sql )
install(TARGETS cxx20sqlm RUNTIME DESTINATION bin)
