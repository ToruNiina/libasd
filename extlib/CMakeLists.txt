if(EXISTS "${PROJECT_SOURCE_DIR}/extlib/pybind11/CMakeLists.txt")
    message(STATUS "pybind already updated.")
else()
    message(STATUS "downloading pybind11 to ./extlib/pybind11 ...")

    execute_process(COMMAND git submodule init
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_QUIET ERROR_QUIET)

    execute_process(COMMAND git submodule update
        WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
        OUTPUT_QUIET ERROR_QUIET)

    message(STATUS "done.")
endif()

add_subdirectory(pybind11)
