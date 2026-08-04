# ARCHITECTURE.md — 项目架构和模块边界 (cpp-001)

## 总体分层（严格遵守）

```
main.cpp (仅组装 + 启动、日志初始化存根)
    |
    v
server/     -> 生命周期、启动/停止
router/     -> 路由注册与分发
controller/ -> 请求/响应 I/O 转换（无业务逻辑）
    |
    v
service/    -> 业务逻辑（纯函数或类）
    |
    v
repository/ -> 数据访问抽象
    |
    v
model/      -> 实体、DTO、值对象
```

横切关注点（common 层）：

* `config/` ：配置加载、校验、默认值（绝不硬编码）
* `common/` ：
  - `Status`：错误/结果处理（值语义、工厂方法、无异常路径）
  - `Logger`：日志（std::chrono 时间戳 + ostream 注入，易测试）
  - 其他工具函数（未来扩展）
  - 原则：仅标准库、值语义优先、RAII、无裸指针
* `tests/`  ：直接测试各层真实实现（链接 .cpp 而非 mock）

横切原则：common 是最底层，被上层依赖但不反向依赖。Logger 等可被所有层使用，但当前仅在测试中验证。

## 当前状态

- 薄 main（仅启动、配置组装、日志初始化存根、server 生命周期）。
- common/ 层已包含：
  - `Status`：值语义错误处理（RAII 友好、无裸指针）。
  - `Logger`：最小日志封装（基于 std::chrono 时间戳 + ostream 注入，支持 Debug/Info/Warn/Error 级别）。
- 所有目录结构就位（含 .gitkeep），等待后续按阶段填充业务层。
- 禁止将业务逻辑放在 main.cpp 或 controller 中（严格分层）。

## 模块边界原则

- 每一层只知道下一层的接口（严格单向依赖）。
- common 是最底层，仅依赖标准库（C++17），无其他项目依赖。
- 业务逻辑绝不直接依赖框架细节（通过 repository/service 抽象）。
- common 组件（Status/Logger）采用值语义 + 依赖注入，便于单元测试和未来替换（例如 Logger → spdlog）。
- 演进路径见 LIBRARY_ROADMAP.md（当前处于“标准库专项”阶段）。

## 演进记录
- bootstrap：Status + 基础骨架
- feat/logger：添加 Logger（chrono + ostream），更新本架构文档
- 未来：引入 fmt / spdlog 时更新 common 抽象边界

未来演进在 LIBRARY_ROADMAP.md 阶段中体现。

## 已实现分层状态 (2026-07-06)

- common: Result<T,E> + Status 示例
- config: 骨架 (早期)
- repository: SQLiteCpp 示例 (早期)
- server: 生命周期 + thread + router 持有 + apply_to 调度
- router: 线性表 + dispatch + apply_to + {param} skeleton + GET/POST/PUT/DELETE
- controller: handle + 调用 service
- service: 业务 + 返回 model::Status
- model: Status DTO

集成测试:
- test_server: 完整链路
- test_router: controller + param + 多方法

原则遵守: 薄 main, 分层单向, 值语义, 小步验证。


## 2026-08-04 main 真实 HTTP

- main 作为 composition root：Config + Logger + Server + Controller 路由注册。
- 默认监听 `0.0.0.0:18080`，路由：`GET /health`、`GET /status` → controller → service → model。
- Server::listen 后台线程；`is_running()` 供 main 生命周期循环；SIGINT/SIGTERM 触发 stop。
