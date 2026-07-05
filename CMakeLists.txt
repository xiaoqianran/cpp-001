cmake_minimum_required(VERSION 3.16)
project(cpp-001 VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 接入 fmt（build 任务，最小改动）：使用 FetchContent 拉取轻量 fmt 库（header-only 友好）
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        10.2.1
)
FetchContent_MakeAvailable(fmt)

# 接入 spdlog（基于 fmt，最小改动）
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1
)
FetchContent_MakeAvailable(spdlog)

# 接入 toml++（配置库，最小改动，基于 C++17）
FetchContent_Declare(
  tomlplusplus
  GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)

# Main executable - thin startup only (per rules)
add_executable(cpp-001
    src/main.cpp
    src/common/Logger.cpp
)

target_include_directories(cpp-001 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(cpp-001 PRIVATE fmt::fmt)
target_link_libraries(cpp-001 PRIVATE spdlog::spdlog)

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

# Logger test (最小改动): 直接驱动真实 common::Logger + chrono/ostream
add_executable(test_logger
    tests/test_logger.cpp
    src/common/Logger.cpp
    src/common/Status.cpp
)

target_include_directories(test_logger PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(test_logger PRIVATE fmt::fmt)
target_link_libraries(test_logger PRIVATE spdlog::spdlog)

add_test(NAME logger_test COMMAND test_logger)

# Filesystem test (标准库专项，最小改动): 使用 std::filesystem
add_executable(test_filesystem
    tests/test_filesystem.cpp
    src/common/FilesystemUtils.cpp
)

target_include_directories(test_filesystem PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_test(NAME filesystem_test COMMAND test_filesystem)

# Optional: make the main target the default
# (build with cmake --build build)
# Config test (toml++ 接入)
add_executable(test_config
    tests/test_config.cpp
    src/common/Config.cpp
)

target_include_directories(test_config PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(test_config PRIVATE tomlplusplus::tomlplusplus)

add_test(NAME config_test COMMAND test_config)
