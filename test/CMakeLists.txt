include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git

    # GoogleTest v1.16.0 and v1.17.0 failed with Clang 21.
    # I feel this was fixed in this commit.
    # https://github.com/google/googletest/commit/fa8438ae6b70c57010177de47a9f13d7041a6328
    #
    # So, I specified that the latest commit should be used as of May 14, 2026.
    #
    # To-do: Update this setting when a new tag is created.
    GIT_TAG d72f9c8aea6817cdf1ca0ac10887f328de7f3da2
)

FetchContent_MakeAvailable(googletest)

set(test_exe aoc2017-test)

file(GLOB CXX_TEST_FILES "*_test.cpp")
add_executable(${test_exe})
target_sources(${test_exe} PRIVATE ${CXX_TEST_FILES})
target_link_libraries(${test_exe} PRIVATE gtest_main solvers)

add_test(NAME solutions_test COMMAND ${test_exe})

file(COPY ${CMAKE_SOURCE_DIR}/inputs DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
