cmake_minimum_required( VERSION 3.16)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/detail)

target_sources(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Channel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Error.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/expected.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Error.cpp
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

