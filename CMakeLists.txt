cmake_minimum_required( VERSION 3.16)

project(cop LANGUAGES CXX VERSION 0.0.1 )

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME}
    STATIC
)

# main code
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

add_subdirectory( include )

target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# testing code
add_subdirectory( tests)

set(TESTS_NAME ${PROJECT_NAME}_tests)

add_executable(${TESTS_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp
)

target_link_libraries(${TESTS_NAME}
    PRIVATE
    ${PROJECT_NAME}
    Catch2::Catch2
    trompeloeil
    cop_unittests::cop_unittests
    cop_integrationtests::cop_integrationtests
)

enable_testing()
find_package(Catch2 REQUIRED)
find_package( trompeloeil REQUIRED )

include(CTest) 
include(Catch)
catch_discover_tests(${TESTS_NAME})


