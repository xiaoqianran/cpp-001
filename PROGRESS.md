# PROGRESS.md — 进展记录 (cpp-001)

记录每次小步推进、commit、学习要点和验证结果。

## 2026-07-06 Bootstrap

- 目标：建立符合所有规则的初始可构建+可测试骨架。
- 执行：
  - 检查 git、主目录仅 LICENSE。
  - 创建 worktree: cpp-001-task-001 (branch task/bootstrap)
  - 创建完整目录树（include/ src/ ... 各分层）。
  - 实现 `common::Status`（值语义、RAII 友好、无裸指针）。
  - 实现薄 `main.cpp`（仅启动/存根日志/配置/服务器生命周期，无业务逻辑）。
  - 最小 CMakeLists.txt（C++17、两个 target、enable_testing、add_test 直接驱动真实实现）。
  - tests/test_status.cpp 直接调用真实 Status::Ok / Error ，测试 copy/move/value 语义。
  - 运行验证命令两次：cmake + build + ctest 全部 0 退出。
  - 创建 8 个必需文档（AGENTS verbatim 规则等）。
  - 提交使用 `chore: ...`
  - 合并回 main，删除 worktree。
- 学习要点：
  - 严格分层与 main.cpp 约束。
  - 值语义 + Status 模式作为 C++ 错误处理起点（替代异常在某些路径）。
  - CMake 最小改动 + ctest 原生集成测试真实代码。
- 验证：两次构建测试通过，主程序可执行输出启动消息。git 历史干净，符合阿里规范前缀。
- 下一步：见 TASKS.md 中的下一个小任务（feat: 基础日志封装）。
- 2026-07-06 post-merge docs update: 标记 bootstrap 完成，明确列出下一最小任务。工作区干净，历史含两个 chore/合并提交。

## 2026-07-06 feat: 基础日志封装

- 目标：添加最小 std:: 实现的 Logger 到 common/ 层，作为后续日志/格式化库的占位。
- 执行：
  - 读取 TASKS.md，选择最小任务。
  - 创建 worktree cpp-001-task-002 (branch task/logger)。
  - 实现 common::Logger + Logger.cpp（chrono 时间戳、4 级 LogLevel、ostream 注入、string_view）。
  - tests/test_logger.cpp 直接调用真实实现，使用 ostringstream 验证输出格式和内容，同时协同 Status。
  - 最小修改 CMakeLists.txt 添加 test_logger target 和 add_test。
  - 工作树内执行 cmake + build + ctest （100% 通过，包括 logger_test）。
  - feat: 提交，合并回 main，删除 worktree。
  - 更新文档、全局记忆。
- 学习要点（C++ 知识）：
  - std::chrono::system_clock + put_time + localtime_r (平台兼容)。
  - string_view 高效传参。
  - 通过 ostream& 依赖注入实现可测试性（无需真实 IO）。
  - enum class 值语义。
  - 头文件默认参数需包含必要头（<iostream>）。
  - common 层横切关注点（Logger + Status 可组合）。
- 验证：ctest 通过，主程序仍正常构建运行。符合所有硬规则（无业务进 main、无裸指针）。
- 全局记忆已更新。

## 2026-07-06 docs: 完善 ARCHITECTURE.md

- 目标：记录 Logger 作为横切关注点，更新当前状态和模块边界。
- 执行：创建 worktree、编辑 ARCHITECTURE.md（分层图、common 详细描述、演进记录）、验证构建测试 100% 通过、docs commit、合并。
- 学习要点：架构文档必须随代码演进同步更新；横切组件（Logger）职责清晰描述有助于后续分层。
- 验证：ctest 全绿，主程序构建正常。
- 全局记忆已同步。

## 2026-07-06 feat: main 中集成 Logger

- 目标：将 Logger 集成到主程序启动路径（极薄，仅替换启动日志输出）。
- 执行：
  - 使用预创建 worktree cpp-001-task-004。
  - 最小修改 main.cpp：引入 Logger，替换 cout 为 logger.log(Info, ...) 。
  - 最小 CMake：将 Logger.cpp 加入 cpp-001 target。
  - worktree 内构建 + ctest 全绿，主程序输出带时间戳日志。
  - feat commit，合并，删除 worktree。
  - 更新 TASKS/PROGRESS。
- 学习要点：main 保持极薄（仅启动/日志）；Logger 依赖注入使 main 易测试和演进。
- 验证：ctest 100%，主程序输出正确格式日志。
- 全局记忆更新。

## 2026-07-06 feat: 标准库专项 - filesystem 练习

- 目标：练习 std::filesystem，添加最小 FilesystemUtils（current_working_directory）。
- 执行：新 worktree、实现 .hpp/.cpp + 测试（直接调用真实实现）、最小 CMake 添加 test、构建测试 100% 通过、feat commit、合并。
- 学习要点：std::filesystem::current_path() 简单易用；返回 string 值语义；测试直接链接 .cpp 。
- 验证：新增 filesystem_test 通过。
- 全局记忆已更新。

## 总结

初始模板 + Logger 集成 + std::filesystem 练习完成。持续小步推进中。

## 2026-07-06 build: 接入 fmt 做格式化

- 目标：使用 CMake FetchContent 接入 fmt 库，实现格式化功能替换 Logger 中的手动字符串拼接。
- 执行：
  - 创建 worktree cpp-001-task-fmt (branch task/fmt)
  - 最小修改 CMakeLists.txt：添加 FetchContent fmt 10.2.1，target_link_libraries 到 cpp-001 和 test_logger。
  - 修改 Logger.cpp：添加 <fmt/format.h>，log() 使用 fmt::format 输出日志行。
  - worktree 内 cmake -S . -B build && cmake --build build && ctest 全绿（包括 fmt 编译）。
  - build commit，合并回 main，删除 worktree。
  - 更新 TASKS.md / PROGRESS.md。
- 学习要点（C++ 知识）：
  - FetchContent 是现代 CMake 标准方式引入第三方库（无 vcpkg/conan）。
  - fmt::format 提供类型安全、高性能格式化，替代 ostringstream 拼接。
  - 头文件包含和链接最小改动保持规则。
  - fmt 支持 C++17，header-only 模式友好。
- 验证：ctest 100% 通过，主程序和测试正常运行，fmt 成功集成。
- 全局记忆已更新。

## 2026-07-06 build: 接入 spdlog 做日志（基于 fmt）

- 目标：引入 spdlog 作为真实日志后端，基于 fmt，保持测试兼容。
- 执行：worktree task/spdlog，CMake FetchContent spdlog v1.14.1，Logger 混合实现（默认 spdlog console，测试路径保留 fmt），测试通过，commit 合并。
- 学习要点：spdlog 依赖 fmt、sink 机制、pattern、level 映射、与现有 ostream 注入兼容的混合策略。
- 验证：ctest 100%。

## 2026-07-06 build: 接入 toml++ 做配置

- 目标：引入 toml++ 作为配置解析库，添加最小 Config 封装。
- 执行：worktree task/tomlpp，CMake FetchContent tomlplusplus v3.4.0，创建 common::Config（parse + get_string/get_int），tests/test_config.cpp 驱动真实实现，构建测试全绿。
- 学习要点：toml++ 现代 C++17 API（table、value<T>）、header-only、in-memory parse 便于测试、类型安全 optional。
- 验证：新增 config_test 通过，所有测试 100%。
- 文档更新：LEARNING_NOTES + TASKS + PROGRESS。

## 2026-07-06 build: 接入 nlohmann/json 做序列化

- 目标：引入 nlohmann/json 作为 JSON 序列化库。
- 执行：worktree task/json，FetchContent nlohmann/json v3.11.3，新增 tests/test_json.cpp 直接练习 parse/dump，全部测试通过。
- 学习要点：现代 C++ JSON（operator[]、dump、parse）、header-only、多平台。
- 验证：json_test 通过，总测试 100%。

## 2026-07-06 build: 接入 cpp-httplib 做 HTTP

- 目标：引入轻量 cpp-httplib 作为 HTTP 入门库。
- 执行：FetchContent v0.18.1，test_httplib 演示 server/client 交互，构建测试全绿。
- 学习要点：单头文件 HTTP server/client、路由 lambda、线程启动。
- 验证：httplib_test 通过。

## 2026-07-06 build: 接入 SQLiteCpp 做数据库

- 目标：引入 SQLiteCpp 进行数据库练习。
- 执行：FetchContent，test_sqlite 使用 :memory: 执行 DDL/DML，验证通过。
- 学习要点：RAII Database/Statement、参数绑定、列获取。
- 验证：sqlite_test 通过。

## 2026-07-06 feat: 薄 main 集成配置+日志演示

- 目标：将已接入的 Config + Logger 薄集成到 main.cpp（遵守规则）。
- 执行：更新 CMake 为 cpp-001 添加 Config.cpp 和 toml link；main.cpp 加载 demo toml 并日志输出。
- 验证：./build/cpp-001 成功输出配置值，所有 ctest 100%。

## 2026-07-06 总结：阶段3-5 库集成 + 薄集成

已自主完成多个小步任务（无用户干预）：
- fmt + spdlog（日志/格式化）
- toml++（配置）
- nlohmann/json（序列化）
- cpp-httplib（HTTP）
- SQLiteCpp（数据库）
- 薄 main 集成 Config + Logger 演示

所有任务：worktree -> build -> test 100% -> commit -> merge -> docs。
当前测试：7 个通过。
项目状态：干净，main 可运行，显示配置加载。

## 2026-07-06 feat: 添加 Result<T,E> 错误处理封装（common）

- 目标：添加纯 C++17 Result 类型到 common，提高错误处理。
- 执行：include/common/Result.hpp (variant 实现)，tests/test_result.cpp。
- 验证：result_test 通过，所有 8 个测试 100%。

## 2026-07-06 feat: 添加 Result<T,E> 错误处理封装（common）

- 目标：纯 C++ 实现 Result 用于错误处理。
- 执行：variant 模板，测试通过。
- 验证：8 个测试全绿。

## 2026-07-06 feat: 使用 Result 改进 Config 返回（common）

- 目标：将 Config 的 get 方法从 optional 改为 Result，提高错误处理显式性。
- 执行：更新 Config.hpp/cpp 使用 Result<std::string,std::string> 和 Result<int,std::string>；修复 Result 实现避免歧义；更新 test_config 和 main 使用新 API。
- 验证：config_test + result_test 通过，main 运行正常输出配置。

## 2026-07-06 feat: 使用 Result 改进 Config 返回（common）

- 目标：将 Config get 方法改为使用 Result（显式错误处理）。
- 执行：更新 Config / Result / test / main；修复 Result 歧义问题。
- 验证：config_test、result_test 通过，main 正常运行。

## 2026-07-06 build: 接入 yaml-cpp 做配置

- 目标：引入 yaml-cpp 作为另一种配置格式库。
- 执行：FetchContent 0.8.0，tests/test_yaml.cpp 演示 YAML 解析。
- 验证：yaml_test 通过。

## 2026-07-06 build: 接入 yaml-cpp 做配置

- 目标：完成阶段 3 配置库，引入 yaml-cpp。
- 执行：FetchContent 0.8.0，test_yaml 演示解析。
- 验证：yaml_test 通过。

## 2026-07-06 feat: 为 Result 添加简单链式操作 (value_or / map)

- 目标：为 Result 增加实用链式方法。
- 执行：添加 value_or 和 map 模板方法；更新测试验证。
- 验证：result_test 通过。

## 2026-07-06 feat: 标准库专项 - 并发练习（添加 ThreadSafeCounter）

- 目标：练习 std::thread + std::mutex。
- 执行：添加 ThreadSafeCounter 头文件 + 多线程测试。
- 验证：concurrency_test 通过（4线程各增1000次，结果正确）。

## 2026-07-06 feat: 标准库专项 - 并发练习（添加 ThreadSafeCounter）

- 目标：练习基础并发。
- 执行：ThreadSafeCounter + 多线程测试。
- 验证：concurrency_test 100% 通过。

## 2026-07-06 feat: 标准库并发 - 添加 AtomicCounter（std::atomic 练习）

- 目标：对比 mutex 与 atomic。
- 执行：AtomicCounter（relaxed memory order）+ 相同多线程测试。
- 验证：atomic_test 通过。

## 2026-07-06 feat: 标准库并发 - 添加 AtomicCounter（std::atomic 练习）

- 目标：无锁计数器练习。
- 执行：AtomicCounter + 多线程测试。
- 验证：atomic_test 通过。

## 2026-07-06 feat: repository 层最小示例（使用 SQLiteCpp + Result）

- 目标：展示 repository 层使用 SQLiteCpp + 错误处理。
- 执行：tests/test_repo.cpp 使用 :memory: DB + CRUD 演示。
- 验证：repo_test 通过。

## 2026-07-06 feat: repository 层最小示例（使用 SQLiteCpp + Result）

- 目标：为后续分层打基础。
- 执行：test_repo 演示 SQLiteCpp 基本用法。
- 验证：repo_test 通过。

## 2026-07-06 feat: 添加最小 HTTP server 示例（使用 cpp-httplib）

- 目标：演示 HTTP server + client 在测试中的使用。
- 执行：Server 设置 /hello handler，线程中 listen，Client 请求验证。
- 验证：httpserver_test 通过。

## 2026-07-06 feat: 添加最小 HTTP server 示例（使用 cpp-httplib）

- 目标：HTTP 阶段进阶。
- 执行：线程中启动 server + client 自测。
- 验证：httpserver_test 通过。
