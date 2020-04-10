cmake_minimum_required( VERSION 3.16)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/cop)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
