project(libasd)
cmake_minimum_required(VERSION 3.0)
enable_testing()

add_definitions("-Wall -Wextra")

if(${ON_TRAVIS})
    message(STATUS "on travis")
    include_directories("${PROJECT_BINARY_DIR}/boost_1_66_0/boost")
endif()

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(test)
