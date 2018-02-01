project(libasd)
cmake_minimum_required(VERSION 3.0)
enable_testing()

add_definitions("-Wall -Wextra")

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(test)
