cmake_minimum_required( VERSION 3.16)

project(cop_unittests)

add_library(${PROJECT_NAME} INTERFACE)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

set(CMAKE_CXX_STANDARD 20)

target_sources(${PROJECT_NAME}
    PRIVATE
)

