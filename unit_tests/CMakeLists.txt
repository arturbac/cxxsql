set( CXXSQL_INCLUDES ${PROJECT_SOURCE_DIR}/include )

add_custom_target( unit_tests )

function(add_compile_test_common HEADER_FILE_NAME EXPECT_FAIL)
  get_filename_component(TEST_NAME ${HEADER_FILE_NAME} NAME_WE)
  set( SOURCE_FILE_NAME test_${TEST_NAME}.cc )
  set( GEN_SOURCE_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE_NAME} )
  
  file(WRITE ${GEN_SOURCE_FILE_NAME} "#include <${HEADER_FILE_NAME}.h>\nint main( int argc, char ** argv )\n {return cxxsql::${HEADER_FILE_NAME}_unit_tests() ? EXIT_SUCCESS : EXIT_FAILURE; }\n" )
  
  add_executable(${TEST_NAME}_cxx20 ${GEN_SOURCE_FILE_NAME})
  target_link_libraries(${TEST_NAME}_cxx20 PRIVATE cxxsql::cxxsql )

  target_include_directories(${TEST_NAME}_cxx20 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/header_only )
  set_target_properties(${TEST_NAME}_cxx20 PROPERTIES
                      EXCLUDE_FROM_ALL TRUE
                      EXCLUDE_FROM_DEFAULT_BUILD TRUE)

  add_test(NAME ${TEST_NAME}
         COMMAND ${CMAKE_COMMAND} --build . --target ${TEST_NAME}_cxx20 --config $<CONFIGURATION>
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  if(EXPECT_FAIL)
    set_tests_properties(${TEST_NAME} PROPERTIES WILL_FAIL TRUE)
  endif()
endfunction()

function(add_compile_test HEADER_FILE_NAME )
  add_compile_test_common( ${HEADER_FILE_NAME} FALSE )
endfunction()

function(add_fail_compile_test HEADER_FILE_NAME )
  add_compile_test_common( ${HEADER_FILE_NAME} TRUE)
endfunction()

function(add_runtime_test TEST_NAME )

  set( SOURCE_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/runtime/${TEST_NAME}.cc )
  
  add_executable(${TEST_NAME}_cxx20_rt ${SOURCE_FILE_NAME})
  
  add_dependencies( unit_tests ${TEST_NAME}_cxx20_rt )
  
  target_compile_options( ${TEST_NAME}_cxx20_rt PRIVATE -Wno-exit-time-destructors )
  target_link_libraries( ${TEST_NAME}_cxx20_rt 
    PRIVATE 
      cxxsql::cxxsql
      Boost::ut
    )
  add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}_cxx20_rt )
endfunction()

add_compile_test( column_constraints )
set_tests_properties(column_constraints   PROPERTIES FIXTURES_SETUP syntax_decl )

add_compile_test( table_decl )
set_tests_properties(table_decl PROPERTIES DEPENDS column_constraints )

add_fail_compile_test( fail_table_decl )
set_tests_properties(fail_table_decl PROPERTIES DEPENDS table_decl )

add_fail_compile_test(fail_column_constraints )
set_tests_properties(fail_column_constraints PROPERTIES DEPENDS table_decl )

set_tests_properties(table_decl   PROPERTIES FIXTURES_SETUP syntax_decl )
set_tests_properties(fail_table_decl   PROPERTIES FIXTURES_SETUP syntax_decl )

add_runtime_test( create_table )
set_tests_properties(create_table PROPERTIES FIXTURES_REQUIRED syntax_decl)

add_runtime_test( crtp_explained )


