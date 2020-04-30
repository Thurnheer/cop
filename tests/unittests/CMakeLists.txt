cmake_minimum_required( VERSION 3.16)

project(cop_unittests)

add_library(${PROJECT_NAME} SHARED)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

set(CMAKE_CXX_STANDARD 20)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/HandlerWrapperTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChannelImplTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BinaryFramerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/HeaderTest.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
    cop::cop
)

target_compile_options( ${PROJECT_NAME}
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wnon-virtual-dtor
    -pedantic
    -Wold-style-cast
    -Wcast-align
    -Wunused
    -Woverloaded-virtual
    -Wconversion
    -Wsign-conversion
    -Wmisleading-indentation
    -Wduplicated-cond
    -Wduplicated-branches
    -Wlogical-op
    -Wnull-dereference
    -Wuseless-cast
    -Wdouble-promotion
    -Wformat=2
    -Weffc++
)

