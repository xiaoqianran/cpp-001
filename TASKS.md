# TASKS.md — 任务队列 (cpp-001)

本文件跟踪待办、进行中和已完成的小步可验证任务。
每个任务必须：能构建、能测试、能提交、能总结。

## 当前任务

- [ ] build: 接入 SQLiteCpp 做数据库（build: 接入 SQLiteCpp）
- [x] build: 接入 cpp-httplib 做 HTTP（build: 接入 cpp-httplib）
- [x] build: 接入 nlohmann/json 做序列化（build: 接入 nlohmann/json）
- [x] build: 接入 toml++ 做配置（build: 接入 toml++）
- [x] build: 接入 spdlog 做日志（基于 fmt）
- [x] build: 接入 fmt 做格式化（build: 接入 fmt）
- [x] docs: 更新 LEARNING_NOTES 添加 std::filesystem 条目 + 全局记忆
- [x] feat: 标准库专项 - filesystem 练习（添加最小 FilesystemUtils，使用 std::filesystem 获取路径，带单元测试）
- [x] feat: main 中集成 Logger（保持极薄启动，仅初始化演示 + 时间戳输出）
- [x] docs: 完善 ARCHITECTURE.md 各层详细职责（记录 Logger 横切关注点）
- [x] feat: 基础日志封装（在 common 中添加最小 Logger，使用 std::chrono / ostream 起步，带单元测试，可构建可运行）
- [x] bootstrap: 初始化 C++ 后端项目骨架、八个文档、目录结构、最小 CMake + 真实单元测试（Status value type in common）

## 已完成

- build: 接入 SQLiteCpp 做数据库 (2026-07-06): FetchContent SQLiteCpp 3.3.1 + sqlite3，in-memory DB 测试，CRUD 演示，测试通过。

- build: 接入 cpp-httplib 做 HTTP (2026-07-06): FetchContent cpp-httplib v0.18.1，tests/test_httplib.cpp 演示 Server + Client 基本交互，测试通过。

- build: 接入 nlohmann/json 做序列化 (2026-07-06): FetchContent nlohmann/json v3.11.3，测试直接使用 nlohmann::json 解析/序列化，构建测试全绿。

- build: 接入 toml++ 做配置 (2026-07-06): FetchContent tomlplusplus v3.4.0，新增 common::Config 最小封装，支持字符串/整数获取，带单元测试，全绿。

- build: 接入 spdlog 做日志 (2026-07-06): FetchContent spdlog v1.14.1 (依赖 fmt)，Logger 混合使用 spdlog（默认 console）+ fmt（测试 ostream 注入），构建测试全绿。

- build: 接入 fmt 做格式化 (2026-07-06): 使用 FetchContent 引入 fmt 10.2.1，最小 CMake 改动，Logger 使用 fmt::format 重构日志输出，构建测试全绿，commit 合并。

- bootstrap (2026-07-06): 创建 worktree、结构、Status 纯逻辑单元、main 薄启动、CMake、测试通过、文档、commit 合并。

## 待办队列（按推荐顺序，小步）

1. `feat: 基础日志封装` （引入或封装简单日志，后续接入 spdlog）
2. `docs: 完善 ARCHITECTURE.md 各层详细职责`
3. 标准库专项小功能（e.g. filesystem 练习）
4. 接入 fmt 做格式化（build: 接入 fmt）
5. ... (后续从 LIBRARY_ROADMAP 按阶段拆分)

## 规则

- 永远从 TASKS.md 挑选**最小**可完成任务。
- 完成一个任务后立即更新本文件 + PROGRESS.md + git commit。
- 禁止大任务或跨阶段混合。
