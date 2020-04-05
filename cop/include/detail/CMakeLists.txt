cmake_minimum_required( VERSION 3.16)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/HandlerWrapper.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InPlaceAllocationPolicy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DynamicMemoryPolicy.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ChannelImpl.hpp
)
