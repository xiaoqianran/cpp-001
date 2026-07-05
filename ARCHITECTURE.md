# ARCHITECTURE.md — 项目架构和模块边界 (cpp-001)

## 总体分层（严格遵守）

```
main.cpp (仅组装 + 启动)
    |
    v
server/     -> 生命周期、启动停止
router/     -> 路由注册与分发
controller/ -> I/O 转换（请求/响应）
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

横切：

* `config/` ：配置加载、校验、默认值（绝不硬编码）
* `common/` ：Status/Result、错误码、日志包装、工具函数（值语义优先）
* `tests/`  ：直接测试各层真实实现

## 当前状态（bootstrap）

- 只有薄 main + common::Status 作为错误处理基础。
- 所有其他目录为空占位，等待后续任务按阶段填充。
- 禁止将逻辑放在 main.cpp 或 controller 中。

## 模块边界原则

- 每一层只知道下一层的接口。
- common 是最底层，无其他项目依赖。
- 业务逻辑绝不直接依赖 HTTP 框架或 DB 驱动细节（通过抽象）。

未来演进在 LIBRARY_ROADMAP.md 阶段中体现。
