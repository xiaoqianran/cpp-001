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
