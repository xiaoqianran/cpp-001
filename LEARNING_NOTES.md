# LEARNING_NOTES.md — 每个库的学习笔记 (cpp-001)

每个引入的库在此追加一个完整条目。

---

## 模板格式（必须严格遵循）

```markdown
## 库名：xxx

### 所属分类
来自 awesome-cpp 的哪个方向。

### 解决的问题
这个库主要解决什么问题。

### 本项目如何使用
改了哪些文件，实现了什么功能。

### 最小示例
给出一个可理解的小代码片段。

### C++ 知识点
涉及哪些 C++ 概念：RAII、模板、移动语义、异常安全、并发、IO、CMake、ABI 等。

### 常见坑
安装、链接、平台、生命周期、性能、线程安全等问题。

### 替代方案
同类库有哪些，各自取舍是什么。

### 结论
适合什么项目，不适合什么项目。
```

---

## 当前笔记

（bootstrap 阶段无外部库。第一个库笔记将在引入时追加。）

### 标准库使用记录（Status + Logger 示例涉及）

- 使用了 C++17 class + value semantics。
- static factory + private ctor 惯用法。
- noexcept 正确使用。
- 直接由测试驱动真实 .cpp 实现（链接）。

## 标准库：chrono / string_view / ostream 用于日志

### 所属分类
C++ 标准库专项（chrono, string_view, iostream）—— 对应推荐阶段 2。

### 解决的问题
提供轻量、零依赖的时间戳日志记录。便于早期开发和测试注入。

### 本项目如何使用
- 新增 include/common/Logger.hpp + src/common/Logger.cpp
- tests/test_logger.cpp （使用 stringstream 验证）
- CMakeLists.txt 最小添加 test_logger 目标
- 测试中同时验证与 Status 协同

### 最小示例
```cpp
std::ostringstream oss;
common::Logger log(oss);
log.log(common::LogLevel::Info, "hello");
assert(oss.str().find("[INFO]") != std::string::npos);
```

### C++ 知识点
- std::chrono (system_clock, duration_cast, time_t 转换)
- std::put_time + localtime_r (跨平台)
- std::string_view (零拷贝传参)
- ostream 依赖注入 (测试友好)
- enum class (强类型值语义)
- RAII/值语义优先

### 常见坑
- C++17 下 chrono 格式化仍需手动（无 std::format 直到 C++20）。
- 头文件中默认参数需要 #include 对应头，否则链接/编译错误。
- localtime 线程不安全，生产可用 std::put_time + gmtime 或库。

### 替代方案
- fmt::format + spdlog（后续引入，更现代、高性能）
- Boost.Log （重）
- 纯 std::clog 但缺少级别/时间戳控制

### 结论
非常适合 bootstrap 和学习阶段。生产后端推荐替换为 spdlog + fmt。易于演进（Logger 接口可保持）。

未来每个库都必须有这样结构化的笔记。

## 标准库：std::filesystem

### 所属分类
C++ 标准库专项（filesystem）—— 对应推荐阶段 2。

### 解决的问题
跨平台路径操作、文件系统查询（cwd、存在性、遍历等）。

### 本项目如何使用
- 新增 include/common/FilesystemUtils.hpp + src/common/FilesystemUtils.cpp
- tests/test_filesystem.cpp 直接测试 current_working_directory()
- CMake 添加 test_filesystem target

### 最小示例
```cpp
std::string cwd = common::current_working_directory();
assert(!cwd.empty());
```

### C++ 知识点
- std::filesystem (current_path, path/string 转换)
- 值语义返回 std::string
- C++17 标准库特性
- CMake 链接真实 .cpp 实现

### 常见坑
- filesystem 需要 C++17 支持，某些旧编译器需链接 stdc++fs（现代 gcc/clang 通常内置）。
- 路径编码/平台差异（Windows vs Unix）。
- 异常 vs error_code 重载（本实现用简单版本）。

### 替代方案
- Boost.Filesystem （更老但兼容）
- 手动字符串操作（易错）
- 第三方如 ghc::filesystem （header-only 兼容）

### 结论
现代 C++ 后端必备基础。适合学习和轻量工具；重度文件操作可考虑更高层库。

未来每个库都必须有这样结构化的笔记。

## 库名：fmt

### 所属分类
格式化、日志、配置（来自 awesome-cpp 格式化库分类）。

### 解决的问题
提供现代、高性能、类型安全的字符串格式化，替代 printf/iostream 手动拼接，解决格式化代码繁琐、类型不安全问题。

### 本项目如何使用
- CMakeLists.txt：添加 FetchContent_Declare + MakeAvailable(fmt)，target_link_libraries 到 main 和 test_logger。
- src/common/Logger.cpp：引入 <fmt/format.h>，log() 函数改用 fmt::format 构造日志行。
- 保持最小改动规则，所有测试通过。

### 最小示例
```cpp
#include <fmt/format.h>
std::string line = fmt::format("[{}] [{}] {}\n", timestamp, level, message);
out << line;
```

### C++ 知识点
- CMake FetchContent（现代依赖管理，无需外部包管理器）。
- 模板元编程（fmt 核心是编译期格式字符串检查）。
- 头文件 vs 链接（fmt 可 header-only 或编译库）。
- string_view 高效传参与 fmt 兼容。
- 值语义友好。

### 常见坑
- Git clone 大小（FetchContent 会下载整个 repo，CI 需注意缓存）。
- 版本选择（10.2.1 稳定，与 C++17 完美兼容）。
- 链接时需 fmt::fmt（静态库 libfmtd.a 在 Debug）。
- 平台 localtime_r 仍保留（fmt 不处理时间格式化）。

### 替代方案
- std::format (C++20)：标准但支持晚，编译器支持不全。
- Boost.Format：重量级。
- spdlog（后续会引入，内部使用 fmt）。

### 结论
适合所有需要格式化的后端项目，轻量、零依赖、现代 C++ 首选。几乎无不适合场景，是 fmt 成为事实标准的原因。

## 库名：spdlog

### 所属分类
格式化、日志、配置（awesome-cpp 日志库）。

### 解决的问题
高性能、线程安全、易用的日志库，支持多种 sink、格式化（内部 fmt）、异步等。

### 本项目如何使用
- CMake: FetchContent spdlog (依赖 fmt)。
- Logger: 默认使用 spdlog::stdout_color_mt + pattern；测试时回退 fmt 路径保持断言通过。
- 最小改动，API 不变。

### 最小示例
```cpp
auto logger = spdlog::stdout_color_mt("app");
logger->info("Server starting");
```

### C++ 知识点
- 智能指针 shared_ptr<logger>、sink 工厂。
- 编译期格式 (fmt)。
- 条件编译/运行时分支保持向后兼容。
- CMake 依赖链 (spdlog -> fmt)。

### 常见坑
- ostream_sink 在测试中颜色/格式差异 → 采用混合策略。
- 多个 logger 名称冲突（使用唯一名称）。
- 线程安全默认开启。

### 替代方案
- fmt 直接 + 自定义封装。
- Boost.Log（重量）。
- glog。

### 结论
后端日志首选，与 fmt 完美搭配。适合生产，测试需注意 sink。

## 库名：toml++

### 所属分类
格式化、日志、配置（awesome-cpp 配置/序列化相关）。

### 解决的问题
现代 TOML 解析与序列化，类型安全、零依赖、易用 API，适合应用配置。

### 本项目如何使用
- CMakeLists.txt: FetchContent tomlplusplus v3.4.0，target_link_libraries 到 test_config。
- 新增 include/common/Config.hpp + src/common/Config.cpp：最小封装 toml::parse + .value<T>() + optional。
- tests/test_config.cpp：直接调用真实实现，验证字符串/整数获取。
- 未来可扩展到 src/config/ 层加载文件。

### 最小示例
```cpp
toml::table tbl = toml::parse(R"(port = 8080)");
auto port = tbl["port"].value<int>();
```

### C++ 知识点
- 模板 value<T>() + optional 错误处理（现代 C++）。
- 头文件 only 库接入。
- string_view 传参。
- RAII（table 生命周期）。
- FetchContent 依赖管理。

### 常见坑
- 版本选择（v3.x 推荐，C++17 要求）。
- 嵌套表访问需 tbl["a"]["b"]。
- 解析错误抛异常（需 try/catch 或 check）。
- 与 nlohmann/json 对比：TOML 更适合人类可读配置。

### 替代方案
- yaml-cpp（更复杂）。
- Boost.Program_options（命令行 + 配置）。
- nlohmann/json（JSON 更流行但 TOML 更友好）。

### 结论
后端配置首选之一，轻量现代。非常适合当前阶段学习 CMake + 配置加载。

## 库名：nlohmann/json

### 所属分类
JSON / 序列化（awesome-cpp 序列化方向）。

### 解决的问题
直观、现代、高性能的 JSON 解析与序列化，单头文件。

### 本项目如何使用
- CMake: FetchContent nlohmann/json v3.11.3。
- tests/test_json.cpp：使用 nlohmann::json 构造、dump、parse 验证。
- 后续可用于 model/DTO 序列化、HTTP body 等。

### 最小示例
```cpp
using json = nlohmann::json;
json j = {{"key", 42}};
std::string s = j.dump();
json j2 = json::parse(s);
```

### C++ 知识点
- 用户定义字面量、模板特化。
- 异常 vs 错误码（可选）。
- 移动语义友好。
- CMake FetchContent 单头文件集成。

### 常见坑
- 包含路径：nlohmann/json.hpp
- 大对象性能（但对大多数后端够用）。
- 版本 v3.x 稳定。

### 替代方案
- RapidJSON（更快更底层）。
- simdjson（极快解析）。
- protobuf（二进制强类型）。

### 结论
后端 JSON 事实标准之一。非常适合学习序列化。

## 库名：cpp-httplib

### 所属分类
HTTP / Web 框架（awesome-cpp Web 服务器方向）。

### 解决的问题
极轻量单头文件 HTTP/1.1 server 和 client。

### 本项目如何使用
- CMake FetchContent cpp-httplib。
- tests/test_httplib.cpp：最小 Server + Client 示例。
- 未来可用于 router / server 层基础。

### 最小示例
```cpp
httplib::Server svr;
svr.Get("/ping", [](auto&, auto& res){ res.set_content("pong", "text/plain"); });
svr.listen("0.0.0.0", 8080);
```

### C++ 知识点
- Lambda + 回调。
- 线程 + 同步（listen 在线程）。
- 头文件 only 库。

### 常见坑
- 必须 stop() 才能 join 线程。
- 端口占用、sleep 启动等待。
- 生产建议用更成熟框架（Crow/Drogon）。

### 替代方案
- Crow（类似但更多功能）。
- Boost.Beast（更底层强大）。
- cpp-httplib 适合学习和原型。

### 结论
极佳的 HTTP 学习起点，轻量无依赖。

## 库名：SQLiteCpp

### 所属分类
数据库（awesome-cpp 数据库方向）。

### 解决的问题
C++ 友好的 SQLite 封装，RAII、安全。

### 本项目如何使用
- CMake FetchContent SQLiteCpp。
- tests/test_sqlite.cpp：in-memory DB + Statement。
- 未来 repository 层可扩展。

### 最小示例
```cpp
SQLite::Database db(":memory:");
db.exec("CREATE TABLE ...");
```

### C++ 知识点
- RAII（Database、Statement 自动关闭）。
- 异常安全。
- 编译 sqlite3 源码。

### 常见坑
- 需要 link sqlite3（FetchContent 处理）。
- 事务管理。

### 替代方案
- sqlite_orm（ORM 风格）。
- SOCI。
- 裸 sqlite3 C API。

### 结论
轻量嵌入式 DB 很好起点。

## C++ 概念：Result<T, E>（错误处理）

### 所属分类
标准库专项 / 错误处理（C++17 variant + optional 模式）。

### 解决的问题
类型安全的错误返回，替代异常或 out-param，鼓励显式处理。

### 本项目如何使用
- include/common/Result.hpp
- tests/test_result.cpp 演示 ok/err 分支。
- 未来可用于 repository / service 返回。

### 最小示例
```cpp
Result<int, std::string> r = 42; // ok
if (r.is_ok()) { use(r.value()); }
```

### C++ 知识点
- std::variant（类型安全 union）。
- 值语义、移动。
- 模板。
- 替代 C++23 std::expected。

### 常见坑
- variant 访问需检查 holds_alternative 或使用 visit。
- 错误类型设计（string vs enum）。

### 替代方案
- std::optional（只成功/无值）。
- 异常。
- std::expected (C++23)。
- Boost.Outcome。

### 结论
强烈推荐在后端 common 层使用，提升健壮性。
