#include "common/Logger.hpp"
#include "common/Config.hpp"

// NOTE: main.cpp follows strict rule - ONLY startup, config assembly, logs, server lifecycle.
// NO business logic, NO controllers, NO services here.

int main() {
    common::Logger logger;

    logger.log(common::LogLevel::Info, "cpp-001 C++ backend starting...");

    // 薄集成：使用 toml++ Config 加载演示配置
    std::string demo_toml = R"(
        app_name = "cpp-001"
        port = 8080
    )";
    common::Config cfg(demo_toml);

    if (auto name = cfg.get_string("app_name")) {
        logger.log(common::LogLevel::Info, "[config] app_name = " + *name);
    }
    if (auto port = cfg.get_int("port")) {
        logger.log(common::LogLevel::Info, "[config] port = " + std::to_string(*port));
    }

    logger.log(common::LogLevel::Info, "[log] Logger + spdlog/fmt initialized.");
    logger.log(common::LogLevel::Info, "[server] Server starting (stub)...");

    logger.log(common::LogLevel::Info, "cpp-001 ready (thin integration demo).");

    return 0;
}
