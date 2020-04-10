cmake_minimum_required( VERSION 3.16)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/detail)

target_sources(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/Event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Channel.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Error.hpp

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Error.cpp
)

