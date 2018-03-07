project(libasd)
cmake_minimum_required(VERSION 3.0)
enable_testing()

add_definitions("-Wall -Wextra -Wpedantic")

find_package(Boost)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(STATUS "Boost not found.")
    message(STATUS "If you have Boost (e.g. /opt/boost-1.??.??) and CMake couldn't found Boost, try")
    message(STATUS "`cmake path/to/CMakeLists.txt -DBOOST_ROOT=/path/to/boost`")
endif()

include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(test)
