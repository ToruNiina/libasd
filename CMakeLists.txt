project(libasd)
cmake_minimum_required(VERSION 3.0)
enable_testing()

add_definitions("-Wall -Wextra")

if(${ON_TRAVIS})
    include_directories("${PROJECT_BINARY_DIR}/boost_1_66_0/boost")
    message(STATUS "on travis. ${PROJECT_BINARY_DIR}/boost_1_66_0/boost is added as include_directories.")
endif()

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(test)
