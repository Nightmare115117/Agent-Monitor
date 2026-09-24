include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

add_executable(agent_monitor_tests ${TEST_SOURCES})
target_link_libraries(agent_monitor_tests PRIVATE
    system_lib
    GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(agent_monitor_tests)