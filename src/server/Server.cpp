#include "server/Server.hpp"

namespace server {

Server::Server() = default;

Server::~Server() {
    stop();
}

void Server::route(const std::string& method, const std::string& pattern, Handler handler) {
    router_.add_route(method, pattern, std::move(handler));
}

void Server::listen(const std::string& host, int port) {
    // 集成点：将 Router 中的所有路由应用到内部 httplib Server
    router_.apply_to(svr_);

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
