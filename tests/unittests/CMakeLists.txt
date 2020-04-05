cmake_minimum_required( VERSION 3.16)

project(cop_unittests)

add_library(${PROJECT_NAME} SHARED)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

set(CMAKE_CXX_STANDARD 20)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/HandlerWrapperTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChannelImplTest.cpp
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
    cop::cop
)

