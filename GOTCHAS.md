# GOTCHAS.md — 踩坑、C++ 陷阱、库兼容问题 (cpp-001)

## 初始记录 (bootstrap)

- CMake 构建目录 `build/` 绝不能提交或手动编辑。
- 头文件放置：公共头放 `include/`，源放 `src/`，CMake include_directories 需正确设置，否则链接失败。
- 值语义：确保类有合理的默认构造、拷贝、移动（Status 示例已示范）。
- 测试必须链接**真实**实现文件（不能只链接头或 mock 实现）。
- Git worktree 切换后，始终在对应目录下执行 cmake/build/ctest。
- C++17 特性可用（structured bindings、optional、variant、filesystem 等），但注意编译器支持。
- 避免在头文件中定义非 inline 的非模板函数（ODR 问题）。
- 暂无大型库，故无 ABI、链接顺序、find_package 问题。
- C++17 chrono 格式化：需手动 put_time + localtime_r/gmtime_r，注意平台差异（_s vs _r）和线程安全。生产建议用库。
- 头文件默认参数：使用 std::cout 等必须 #include <iostream> 在声明所在头中。

## 未来添加条目格式

- 问题描述
- 复现/触发条件
- 解决方案
- 相关文件/提交
- 防止再次发生的方法
