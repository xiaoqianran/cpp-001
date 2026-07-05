# LIBRARY_ROADMAP.md — awesome-cpp 学习路线 (cpp-001)

参考：https://github.com/fffaraz/awesome-cpp

**原则**：
- 每轮只引入**一个**新库。
- 每个库必须有：最小 demo + 测试 + LEARNING_NOTES.md 条目。
- 按阶段顺序，GUI/游戏/图形库做独立 demo，不塞主链路。
- 过重/过时/难装的标记“了解即可”。

## 推荐长期阶段（按顺序）

1. **C++ 后端基础模板** (当前完成)
   - CMake、目录结构、分层、Result/Status、配置、最小测试。
   - 库：无（纯标准 + CMake）

2. **标准库专项**
   - STL 容器、filesystem、chrono、thread、mutex、future、optional、variant、string_view。
   - 示例功能：配置加载使用 filesystem + chrono 时间戳等。

3. **格式化、日志、配置**
   - `fmt`、`spdlog`、`toml++`、`yaml-cpp`、`Boost.Program_options`。
   - 第一库建议：fmt（轻量、现代）。

4. **JSON / 序列化**
   - `nlohmann/json`、`RapidJSON`、`simdjson`、`protobuf`、`cereal`。

5. **HTTP / Web 框架**
   - `Boost.Asio`、`Boost.Beast`、`Crow`、`Drogon`、`cpp-httplib`。
   - 注意：选适合后端的轻量或成熟框架。

6. **数据库**
   - `SQLiteCpp`、`sqlite_orm`、`libpqxx`、`SOCI`、`redis-plus-plus`。

7. **并发 / 异步 / 事件循环**
   - `Boost.Asio`、`libuv`、`libevent`、`TBB`、`concurrentqueue`。

8. **RPC / 消息 / IPC**
   - `gRPC`、`ZeroMQ`、`Boost.Interprocess`。

9. **压缩 / 加密 / 安全**
   - `zlib`、`zstd`、`lz4`、`OpenSSL`、`libsodium`。

10. **测试 / Benchmark / 质量工具**
    - `GoogleTest`、`Catch2`、`doctest`、`benchmark`、sanitizers、clang-tidy。

11. **性能 / 内存 / 底层专项**
    - `mimalloc`、`jemalloc`、`abseil`、`folly`、`Boost.Container`。

12. **数学 / 图 / AI 推理**
    - `Eigen`、`Boost.Graph`、`OpenCV`、`ONNX Runtime`、`LibTorch`。（后端侧重工具化 demo）

## 每阶段选择 1-3 库

在每个阶段完成后，更新本文件标记已完成库，并为下个库设计小任务。

当前阶段：1 已完成。下一阶段开始前先更新 TASKS.md 选定第一个库 demo。
