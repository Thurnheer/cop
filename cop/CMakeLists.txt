cmake_minimum_required( VERSION 3.16)

project( cop LANGUAGES CXX VERSION 0.0.1 )

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME}
    STATIC
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME} )

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${PROJECT_NAME}
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
