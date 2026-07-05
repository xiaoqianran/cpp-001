# AGENTS.md — Agent 工作规则 (for cpp-001)

**重要**：这是一个长期陪跑型 C++ 后端工程 Agent 项目。所有操作必须严格遵守以下边界和规则。

## 工作边界

只操作当前打开的 C++ 后端 worktree，不要碰父目录或兄弟项目。

目录约定：

* `{project_name}`：主项目 main（当前为 cpp-001）
* `{project_name}-task-xxx`：任务分支 worktree
* 其他同级目录：无关项目，禁止修改

## 硬规则

* 只改当前目录。
* 不改父目录和兄弟项目。
* 不改 `build/`、`cmake-build-*`、`bin/`、`obj/`。
* 不随意改 `.idea/`、`.vscode/`。
* 不硬编码密钥、数据库地址、绝对路径。
* 不把业务逻辑写进 `main.cpp` 或 controller。
* 不滥用裸指针，优先 RAII、智能指针、标准库容器和值语义。
* 不随意引入大型依赖。
* 不擅自更换 Conan / vcpkg / submodule / package manager 体系。
* 修改 `CMakeLists.txt` 必须保持最小改动。
* 每完成一个小功能必须 git commit。
* 每次提交前尽量通过构建和测试。

## 项目结构

先创建并维护这些文档：

* `AGENTS.md`：Agent 工作规则
* `TASKS.md`：任务队列
* `PROGRESS.md`：进展记录
* `GOTCHAS.md`：踩坑、C++ 陷阱、库兼容问题
* `TESTING.md`：构建、测试、运行方式
* `ARCHITECTURE.md`：项目架构和模块边界
* `LIBRARY_ROADMAP.md`：awesome-cpp 学习路线
* `LEARNING_NOTES.md`：每个库的学习笔记

基础目录：

* `src/main.cpp`
* `include/`
* `app/`
* `tests/`
* `cmake/`
* `config/`
* `src/server/`
* `src/router/`
* `src/controller/`
* `src/service/`
* `src/repository/`
* `src/model/`
* `src/config/`
* `src/common/`

分层规则：

* `main.cpp`：只负责启动、组装配置、日志、server。
* `server`：服务启动、停止、生命周期。
* `router`：路由注册。
* `controller`：请求输入输出转换。
* `service`：业务逻辑。
* `repository`：数据访问。
* `model`：实体、DTO、值对象。
* `config`：配置加载和校验。
* `common`：错误码、Result、日志、通用工具。
* `tests`：单元测试和集成测试。

## 验证命令

默认使用：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build
```

如果已有 clang-format：

```bash
clang-format -i src/**/*.cpp include/**/*.hpp
```

## Git 规则

使用 git worktree 做任务分支。

流程：

1. 检查当前目录和 git 状态。
2. 阅读项目文档。
3. 从 `TASKS.md` 选择一个最小任务。
4. 创建 `{project_name}-task-xxx` worktree。
5. 只完成当前任务。
6. 构建、测试。
7. 更新文档。
8. git commit。
9. 回到 main worktree 合并。
10. 删除已完成 task worktree。
11. 更新 `PROGRESS.md`。

commit 使用阿里规范：

* `feat: 新增 HTTP 服务入口`
* `fix: 修复请求解析异常`
* `refactor: 重构 service 层`
* `docs: 更新 C++ 后端架构说明`
* `test: 添加路由测试`
* `chore: 调整 CMake 配置`
* `build: 接入 fmt 依赖`

## awesome-cpp 学习策略

（见 LIBRARY_ROADMAP.md 和用户目标描述）

不要把所有库一次性塞进项目。每个库必须有 demo、测试、学习笔记。

---

本文件 verbatim 包含了 OBJECTIVE 中定义的边界、规则、git 流程和 commit 规范。
