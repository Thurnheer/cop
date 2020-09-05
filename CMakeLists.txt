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
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# for packaging cop
#include( CMakePackageConfigHelpers )

#set( cop_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include" )

#set( CMAKE_CONFIG_DEST "share/cmake/Modules" )

#configure_package_config_file(
    #"${CMAKE_CURRENT_SOURCE_DIR}/cop-config.cmake.in"
    #"${CMAKE_BINARY_DIR}/cop-config.cmake"
    #INSTALL_DESTINATION "${CMAKE_CONFIG_DEST}"
    #PATH_VARS cop_INCLUDE_DIRS
#)

#export( TARGETS cop_LIBRARIES FILE "${CMAKE_BINARY_DIR}/copTargets.cmake" )

#export( PACKAGE cop )

#set(cop_INCLUDE_DIRS "include" )

#configure_package_config_file(
    #"${CMAKE_CURRENT_SOURCE_DIR}/cop-config.cmake.in"
    #"${CMAKE_BINARY_DIR}/export/cop-config.cmake"
    #INSTALL_DESTINATION "${CMAKE_CONFIG_DEST}"
    #PATH_VARS cop_INCLUDE_DIRS
#)

#install(
    #EXPORT export
    #DESTINATION ${CMAKE_CONFIG_DEST} FILE "copTargets.cmake"
#)

# export cop
#export( TARGETS cop NAMESPACE cop:: FILE copConfig.cmake)

#set( CMAKE_EXPORT_PACKAGE_REGISTRY ON)
#export (PACKAGE cop)

# testing code
if(CMAKE_POJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory( tests )

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
        cop_systemtests::cop_systemtests
    )

    target_compile_options(${TESTS_NAME}
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

enable_testing()
    include(CTest) 
    find_package(Catch2 REQUIRED)
    find_package( trompeloeil REQUIRED )

    include(Catch)
    catch_discover_tests(${TESTS_NAME})

endif()

