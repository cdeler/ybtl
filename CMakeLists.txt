cmake_minimum_required(VERSION 2.8)
project(ybtl C)

set(CMAKE_C_STANDARD 11)

set(PROJECTS_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_TESTS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)

set(LINK_DEPENDENCIES ybtl cmocka dl elf dw)

include_directories(${PROJECTS_DIR})

include(utils/unit_tests.cmake)

add_library(ybtl ybtl_stack_walker.c ybtl_stack_walker.h ybtl.h ybtl_types.h ybtl_dwarf.c ybtl_dwarf.h)

setup_target_for_test(stack_walker_unittest)
setup_target_for_test(dwarf_unittest)
