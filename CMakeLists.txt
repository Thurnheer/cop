cmake_minimum_required( VERSION 3.16)

project(COP_testsAndLib LANGUAGES CXX VERSION 0.0.1 )

set(CMAKE_CXX_STANDARD 20)

enable_testing()

add_subdirectory( cop )

add_subdirectory( tests)

