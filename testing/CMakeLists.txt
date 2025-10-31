# Fetch google test
message(STATUS "Running CMake in /testing")
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

# Create testing executable
add_executable(woven_testing testing_main.cpp)
target_link_libraries(woven_testing PRIVATE woven_lib gtest gmock CURL::libcurl)

#Macro for testing
target_compile_definitions(woven_testing PRIVATE UNIT_TEST)

# CTest
include(GoogleTest)
gtest_discover_tests(woven_testing)
