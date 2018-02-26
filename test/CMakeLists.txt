set(TEST_NAMES
    test_container_dispatches
    test_throw_exception
    test_frame_data
    )

if(${Boost_FOUND})
    add_definitions("-O2 -std=c++11")
    foreach(TEST_NAME ${TEST_NAMES})
        add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach(TEST_NAME)
else()
    message(WARNING
        "Because it couldn't found Boost, all the tests are disabled."
        "If you want to run test in your environment, please install boost.")
endif()
