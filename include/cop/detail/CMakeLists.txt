cmake_minimum_required( VERSION 3.16)

project(cop_detail CXX)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/HandlerWrapper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InPlaceAllocationPolicy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DynamicMemoryPolicy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChannelImpl.hpp
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_compile_options( ${PROJECT_NAME}
    INTERFACE
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
