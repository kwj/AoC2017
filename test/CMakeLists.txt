include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.16.0
)

FetchContent_MakeAvailable(googletest)

set(test_exe aoc2017-test)

file(GLOB_RECURSE CXX_TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*_test.cpp")
add_executable(${test_exe})
target_sources(${test_exe} PRIVATE ${CXX_TEST_FILES})
target_link_libraries(${test_exe} PRIVATE gtest_main solvers)

add_test(NAME solutions_test COMMAND ${test_exe})

file(COPY ${CMAKE_SOURCE_DIR}/inputs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
