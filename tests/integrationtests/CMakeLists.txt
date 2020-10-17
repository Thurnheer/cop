cmake_minimum_required( VERSION 3.16)

project(cop_integrationtests)

add_executable(${PROJECT_NAME} )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/InterpreterTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataLinkLayerTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TransportLinkLayerTest.cpp
)


target_link_libraries( ${PROJECT_NAME}
    PUBLIC
    PRIVATE
    catch_main
    project_options
    project_warnings
    cop::cop
)

catch_discover_tests(
    ${PROJECT_NAME}
    TEST_PREFIX
    "integrationtests."
    EXTRA_ARGS
    -s
    --reporter=xml
    --out=tests.xml
)

