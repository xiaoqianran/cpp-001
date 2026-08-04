# cpp-001

面向 **C++ 后端** 的长期学习 / 工程骨架项目。  
用小步可验证的方式搭建分层 HTTP 服务，并按路线图逐个接入 [awesome-cpp](https://github.com/fffaraz/awesome-cpp) 常用库。

> 标准：C++17 · CMake 3.16+ · 值语义 / RAII · 业务不进 `main` / controller

---

## 功能概览

- 薄启动入口：配置 + 日志 + Server 生命周期
- 分层 HTTP：`server → router → controller → service → repository → model`
- 健康检查 API：`GET /health`、`GET /status`（JSON）
- 内存 SQLite KV（`KvRepository`）作为 repository 示例
- 原生 `ctest` 单元 / 集成测试（驱动真实实现，非 mock）

---

## 架构

```
main.cpp          仅组装与启动（composition root）
    │
    ▼
server/           生命周期、listen / stop
router/           路由表、多方法、{param} skeleton
controller/       请求/响应 I/O（JSON 序列化）
service/          业务逻辑
repository/       数据访问（SQLiteCpp KV）
model/            DTO / 值对象
common/           Status、Result、Logger、Config 等横切能力
```

原则：

- 单向依赖，下层不反向依赖上层
- `common` 为最底层，优先标准库 + 值语义
- 禁止把业务逻辑写进 `main.cpp` 或 controller

更细说明见 [ARCHITECTURE.md](./ARCHITECTURE.md)。

---

## 技术栈

| 类别 | 库 | 用途 |
|------|-----|------|
| 格式化 | [fmt](https://github.com/fmtlib/fmt) | 字符串格式化 |
| 日志 | [spdlog](https://github.com/gabime/spdlog) | 控制台日志 |
| 配置 | [toml++](https://github.com/marzer/tomlplusplus)、[yaml-cpp](https://github.com/jbeder/yaml-cpp) | 配置解析 |
| JSON | [nlohmann/json](https://github.com/nlohmann/json) | 序列化 |
| HTTP | [cpp-httplib](https://github.com/yhirose/cpp-httplib) | Server / Client |
| 数据库 | [SQLiteCpp](https://github.com/SRombauts/SQLiteCpp) | SQLite 访问 |

依赖通过 CMake `FetchContent` 拉取，无需手动安装 vcpkg / Conan。

---

## 目录结构

```text
cpp-001/
├── include/           公共头文件（按层分子目录）
│   ├── common/
│   ├── controller/
│   ├── model/
│   ├── repository/
│   ├── router/
│   ├── server/
│   └── service/
├── src/               实现与 main
├── tests/             真实实现驱动的测试
├── config/            配置文件目录（预留）
├── cmake/             CMake 辅助（预留）
├── AGENTS.md          Agent / 协作规则
├── TASKS.md           任务队列
├── PROGRESS.md        进展记录
├── ARCHITECTURE.md    架构说明
├── LIBRARY_ROADMAP.md 库学习路线
├── LEARNING_NOTES.md  学习笔记
├── GOTCHAS.md         踩坑记录
├── TESTING.md         构建与测试说明
└── CMakeLists.txt
```

---

## 环境要求

- C++17 编译器（已验证 g++ 12）
- CMake ≥ 3.16
- 网络（首次配置时 FetchContent 下载依赖）
- 可选：系统 SQLite；默认使用 SQLiteCpp 内置 sqlite（`-DSQLITECPP_INTERNAL_SQLITE=ON`）

---

## 构建

```bash
cmake -S . -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DSQLITECPP_INTERNAL_SQLITE=ON

cmake --build build -j"$(nproc)"
```

---

## 运行

```bash
./build/cpp-001
```

默认监听 **`0.0.0.0:18080`**（可在 `main.cpp` 内嵌 toml 演示配置中修改）。

```bash
# 健康检查
curl http://127.0.0.1:18080/health
curl http://127.0.0.1:18080/status
```

示例响应：

```json
{"healthy":true,"message":"service layer OK"}
```

`Ctrl+C` 或 `SIGTERM` 优雅退出。

---

## 测试

```bash
ctest --test-dir build --output-on-failure
```

当前覆盖：Status / Logger / Config / Result / filesystem / 并发 / JSON / HTTP / SQLite / 分层 e2e 等，共 **18** 个测试目标。

规则：测试直接链接并调用真实 `.cpp` / 头文件实现，不写平行 mock 逻辑。详见 [TESTING.md](./TESTING.md)。

---

## HTTP 接口

| 方法 | 路径 | 说明 |
|------|------|------|
| GET | `/health` | 健康状态 JSON |
| GET | `/status` | 同上（别名） |

链路：`Server → Router → Controller → Service → KvRepository → model::Status`。

---

## 开发约定

### 任务流程

1. 从 [TASKS.md](./TASKS.md) 选一个**最小**可完成任务  
2. 使用 git worktree 开任务分支（`cpp-001-task-xxx`）  
3. 实现 → 构建 → 测试  
4. 更新 `TASKS.md` / `PROGRESS.md` 等文档  
5. 提交并合并回 `main`  

### Commit 规范（阿里巴巴风格前缀）

```text
feat:     新功能
fix:      缺陷修复
docs:     文档
refactor: 重构（无行为变化）
test:     测试
chore:    杂项
build:    构建 / 依赖
```

示例：

```bash
git commit -m "feat: 添加健康检查接口"
git commit -m "docs: 补充 README 构建说明"
git commit -m "build: 接入 fmt 依赖"
```

### 硬规则摘要

- 只改当前 worktree，不碰无关兄弟项目  
- 不把业务写进 `main` / controller  
- 优先 RAII、智能指针、标准库容器，避免裸指针  
- 不随意引入大型依赖或更换包管理方案  
- 每完成一小步尽量构建、测试后提交  

完整规则见 [AGENTS.md](./AGENTS.md)。

---

## 学习路线

按阶段引入库，**每轮只接入一个**，并配套 demo + 测试 + `LEARNING_NOTES.md` 条目。

已完成（摘要）：

1. 后端基础模板（CMake、分层、Result/Status）  
2. 标准库专项（filesystem、atomic、mutex 等）  
3. fmt / spdlog / toml++ / yaml-cpp  
4. nlohmann/json  
5. cpp-httplib  
6. SQLiteCpp + 正式 `KvRepository`  

待办与下一库见 [LIBRARY_ROADMAP.md](./LIBRARY_ROADMAP.md)、[TASKS.md](./TASKS.md)。

---

## 文档索引

| 文档 | 内容 |
|------|------|
| [AGENTS.md](./AGENTS.md) | 协作边界与硬规则 |
| [ARCHITECTURE.md](./ARCHITECTURE.md) | 分层与模块边界 |
| [TASKS.md](./TASKS.md) | 任务队列 |
| [PROGRESS.md](./PROGRESS.md) | 进展与学习要点 |
| [LIBRARY_ROADMAP.md](./LIBRARY_ROADMAP.md) | awesome-cpp 路线 |
| [LEARNING_NOTES.md](./LEARNING_NOTES.md) | 库学习笔记 |
| [GOTCHAS.md](./GOTCHAS.md) | 踩坑与兼容问题 |
| [TESTING.md](./TESTING.md) | 构建 / 测试 / 运行 |

---

## License

见 [LICENSE](./LICENSE)。
