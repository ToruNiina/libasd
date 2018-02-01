set(TEST_NAMES
    test_container_dispatches
    test_throw_exception
    test_frame_data
    )

add_definitions("-O2 -std=c++11")

foreach(TEST_NAME ${TEST_NAMES})
    add_executable(${TEST_NAME} ${TEST_NAME}.cpp)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach(TEST_NAME)
