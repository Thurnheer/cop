cmake_minimum_required( VERSION 3.16)

project(cop_include)
add_library(${PROJECT_NAME})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        cop_impl::cop_impl
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cop)
