cmake_minimum_required( VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

project(cop_integrationtests)

add_library(${PROJECT_NAME} SHARED) # TODO change this to Object, shared is a workaround because the test registration code does not work with static libraries.

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Event.cpp
)


target_link_libraries( ${PROJECT_NAME}
    PUBLIC
    PRIVATE
    cop::cop
    cop_unittests::cop_unittests
)



