cmake_minimum_required(VERSION 3.14)
project(WovenChallenge)

# Per job requirement minimum is 14, using v17 for compatibility with GTest version 1.17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CURL
find_package(CURL REQUIRED)

# Add include directories
include_directories(include)

# Library with core logic
add_library(woven_lib
        src/woven.cpp
        src/runner.cpp
)
target_include_directories(woven_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Main app executable
add_executable(woven_challenge src/main.cpp)
target_link_libraries(woven_challenge PRIVATE woven_lib CURL::libcurl)

################# Optional Testing ############################
# Disabled by default
option(TESTING "Enable testing" OFF)

if(TESTING)
    # Enable CMake test framework (ctest)
    enable_testing()
    add_subdirectory(testing)
endif()
