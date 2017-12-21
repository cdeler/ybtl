cmake_minimum_required(VERSION 2.8)
project(linked_list C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_TESTS_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)

set(LINK_DEPENDENCIES linked_list cmocka)

add_library(linked_list linked_list.c linked_list.h)
setup_target_for_test(linked_list_unittest)
