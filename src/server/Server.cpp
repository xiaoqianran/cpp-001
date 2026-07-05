#include "server/Server.hpp"

namespace server {

Server::Server() = default;

Server::~Server() {
    stop();
}

void Server::route(const std::string& method, const std::string& pattern, Handler handler) {
    if (method == "GET") {
        svr_.Get(pattern, handler);
    } else if (method == "POST") {
        svr_.Post(pattern, handler);
    }
    // 可扩展其他方法
}

void Server::listen(const std::string& host, int port) {
    running_ = true;
    server_thread_ = std::thread([this, host, port]() {
        svr_.listen(host, port);
    });
}

void Server::stop() {
    if (running_) {
        svr_.stop();
        if (server_thread_.joinable()) {
            server_thread_.join();
        }
        running_ = false;
    }
}

} // namespace server
