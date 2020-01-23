cmake_minimum_required( VERSION 3.16)

project( cop_tests LANGUAGES CXX VERSION 0.0.1 )

set(CMAKE_CXX_STANDARD 20)

find_package(Catch2 REQUIRED)

add_subdirectory( integrationtests )

add_subdirectory( unittests )

add_executable(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
    Catch2::Catch2
    cop_unittests::cop_unittests
    cop_integrationtests::cop_integrationtests
) 

include(CTest) 
include(Catch)
catch_discover_tests(${PROJECT_NAME})
