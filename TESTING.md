# TESTING.md — 构建、测试、运行方式 (cpp-001)

## 默认验证命令（必须成功）

从项目根（cpp-001）执行：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

这些命令必须 exit code 0 。

## 运行主程序

构建后：

```bash
./build/cpp-001
```

应看到启动存根输出，无崩溃。

## 添加新测试

1. 在 `tests/` 下创建 `test_xxx.cpp`
2. 直接 `#include` 并调用 src/ 下的真实实现（使用 include/ 头）。
3. 更新 `CMakeLists.txt` 添加对应的 `add_executable` + `target_include_directories` + `add_test`。
4. 确保 `ctest` 能发现并通过。
5. 提交前本地运行 `ctest` 验证。

## 格式化（若环境有 clang-format）

```bash
clang-format -i src/**/*.cpp include/**/*.hpp
```

## 注意

- 永远从干净状态（删除 build/ 或新 worktree）开始验证。
- 测试必须驱动**真实**代码路径。
- 不要在测试中硬编码预期或重新实现被测逻辑。
