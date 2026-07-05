cmake_minimum_required(VERSION 3.16)
project(cpp-001 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Main executable - thin startup only (per rules)
add_executable(cpp-001
    src/main.cpp
)

target_include_directories(cpp-001 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Enable testing with ctest (native, no external test framework for bootstrap)
enable_testing()

# Test executable that directly exercises the real shipped common logic
add_executable(test_status
    tests/test_status.cpp
    src/common/Status.cpp
)

target_include_directories(test_status PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME status_test COMMAND test_status)

# Optional: make the main target the default
# (build with cmake --build build)
