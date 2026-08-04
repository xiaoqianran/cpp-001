#include "common/Logger.hpp"
#include "common/Config.hpp"
#include "server/Server.hpp"
#include "controller/Controller.hpp"

#include <atomic>
#include <chrono>
#include <csignal>
#include <string>
#include <thread>

// NOTE: main.cpp follows strict rule - ONLY startup, config assembly, logs, server lifecycle.
// Wiring (composition root) is allowed; NO business logic.

namespace {
std::atomic<bool> g_running{true};

void on_signal(int) {
    g_running = false;
}
} // namespace

int main() {
    common::Logger logger;

    logger.log(common::LogLevel::Info, "cpp-001 C++ backend starting...");

    // 薄集成：toml 演示配置（默认 18080，避免与常见 8080 冲突）
    std::string demo_toml = R"(
        app_name = "cpp-001"
        host = "0.0.0.0"
        port = 18080
    )";
    common::Config cfg(demo_toml);

    auto name_res = cfg.get_string("app_name");
    if (name_res.is_ok()) {
        logger.log(common::LogLevel::Info, "[config] app_name = " + name_res.value());
    }

    std::string host = "0.0.0.0";
    auto host_res = cfg.get_string("host");
    if (host_res.is_ok()) {
        host = host_res.value();
    }

    int port = 18080;
    auto port_res = cfg.get_int("port");
    if (port_res.is_ok()) {
        port = port_res.value();
    }
    logger.log(common::LogLevel::Info,
               "[config] listen = " + host + ":" + std::to_string(port));

    // 组装：server + controller（controller 内部再调 service/model）
    server::Server app;
    controller::Controller ctrl;

    app.route("GET", "/health", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });
    app.route("GET", "/status", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    std::signal(SIGINT, on_signal);
    std::signal(SIGTERM, on_signal);

    app.listen(host, port);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!app.is_running()) {
        logger.log(common::LogLevel::Error,
                   "[server] failed to bind " + host + ":" + std::to_string(port));
        return 1;
    }

    logger.log(common::LogLevel::Info, "[server] listening (GET /health, GET /status)");
    logger.log(common::LogLevel::Info, "cpp-001 ready. Ctrl+C to stop.");

    while (g_running && app.is_running()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    logger.log(common::LogLevel::Info, "[server] shutting down...");
    app.stop();
    logger.log(common::LogLevel::Info, "cpp-001 stopped.");
    return 0;
}
