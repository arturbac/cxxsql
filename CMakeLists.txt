cmake_minimum_required(VERSION 3.20)

project(cxx20sql)

set(CMAKE_CXX_STANDARD 20 )
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF )
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
