function(create_test name)

  set(prefix ARG)
  set(flags NO_EXCEPTIONS WITH_MAIN)
  set(one_value_options "")
  set(multiple_value_options FILES LIBS INCLUDE)

  cmake_parse_arguments(PARSE_ARGV 1 ${prefix} "${flags}"
                        "${one_value_options}" "${multiple_value_options}")

  add_executable("${name}" ${ARG_FILES})

  target_compile_features("${name}" PRIVATE cxx_std_20)
  set_target_properties("${name}" PROPERTIES CXX_EXTENSIONS OFF)

  target_include_directories("${name}" PRIVATE "${ARG_INCLUDE}"
                                               ${DOCTEST_INCLUDE_DIR})
  target_link_libraries("${name}" ${ARG_LIBS})

  target_compile_definitions(
    "${name}"
    PRIVATE
      $<$<BOOL:${ARG_NO_EXCEPTIONS}>:DOCTEST_CONFIG_NO_EXCEPTIONS_BUT_WITH_ALL_ASSERTS>
      $<$<BOOL:${ARG_WITH_MAIN}>:DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN>)

  add_test(NAME "${name}" COMMAND "${name}")

endfunction()
