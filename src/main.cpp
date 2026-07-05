#include "common/Logger.hpp"

// NOTE: main.cpp follows strict rule - ONLY startup, config assembly, logs, server lifecycle.
// NO business logic, NO controllers, NO services here.

int main() {
    common::Logger logger;  // 使用 common::Logger 进行启动日志（极薄集成）

    logger.log(common::LogLevel::Info, "cpp-001 C++ backend starting...");

    // Stub: assemble config (later from src/config/)
    logger.log(common::LogLevel::Info, "[config] Configuration loaded (stub).");

    // Stub: initialize logging
    logger.log(common::LogLevel::Info, "[log] Logger initialized.");

    // Stub: server lifecycle (later in src/server/)
    logger.log(common::LogLevel::Info, "[server] Server starting (stub)...");
    logger.log(common::LogLevel::Info, "[server] Listening on 0.0.0.0:8080 (stub).");

    // In real: server.run() would block here.
    logger.log(common::LogLevel::Info, "cpp-001 ready (stub - bootstrap).");

    return 0;
}
