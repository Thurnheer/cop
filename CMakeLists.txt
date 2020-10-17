cmake_minimum_required( VERSION 3.16)

project(cop LANGUAGES CXX VERSION 0.0.1 )
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    target_compile_options(project_options
        PUBLIC
            -stdlib=libc++
    )
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if(ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif()
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

#enable cache system
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

#enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

#Set up some extra Conan dependencies
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

include(cmake/Conan.cmake)
run_conan()

target_sources(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/BinaryCoder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/BinaryFramer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Channel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Command.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Crc.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/DataLinkLayer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Error.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Event.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Field.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Framer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Header.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Id.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Interpreter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/TransportLinkLayer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/expected.hpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/Error.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/ChannelImpl.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/DynamicMemoryPolicy.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/Empty.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/HandlerWrapper.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/InPlaceAllocationPolicy.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/cop/detail/Utilities.hpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        project_options
        project_warnings
)

target_compile_features(${PROJECT_NAME}
    PRIVATE
        cxx_std_20
)


# testing code
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    option(ENABLE_TESTING "Enable Test Builds" ON)
    option(ENABLE_SHARED_LIBS "Enable compilation of shared libraries" ON)
    option(ENABLE_FUZZING "Enable Fuzzing Buids" OFF)

    enable_testing()

    if(CONAN_CATCH2_ROOT_DEBUG)
        include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
    else()
        include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
    endif()
    
    add_subdirectory( tests )

    add_library(catch_main STATIC ${CMAKE_CURRENT_SOURCE_DIR}/tests/catch_main.cpp)
    add_library(catch_main::catch_main ALIAS catch_main)
    target_compile_features(catch_main PUBLIC cxx_std_20)
    target_link_libraries(catch_main
        PUBLIC
            CONAN_PKG::catch2
            CONAN_PKG::trompeloeil
            project_options
            project_warnings
    )

    include(CTest) 

endif()

