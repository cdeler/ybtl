enable_testing()

add_custom_target(build_tests)

function(setup_target_for_test _test_name)
    message("Added '${_test_name}' test")
    add_executable(${_test_name} tests/${_test_name})

    set_target_properties(${_test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tests")
    target_link_libraries(${_test_name} ${LINK_DEPENDENCIES})

    add_test(${_test_name} ${CMAKE_TESTS_OUTPUT_DIRECTORY}/${_test_name})
    set_tests_properties(${_test_name} PROPERTIES TIMEOUT 5)

    add_dependencies(build_tests ${_test_name})
endfunction()