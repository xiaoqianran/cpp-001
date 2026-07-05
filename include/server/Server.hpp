#pragma once

#include <httplib.h>
#include <functional>
#include <string>
#include <thread>
#include <atomic>

namespace server {

/**
 * 最小 server 层封装（基于 cpp-httplib）。
 * 职责：启动/停止 HTTP server，注册路由。
 */
class Server {
public:
    using Handler = std::function<void(const httplib::Request&, httplib::Response&)>;

    Server();
    ~Server();

    void route(const std::string& method, const std::string& pattern, Handler handler);
    void listen(const std::string& host, int port);
    void stop();

private:
    httplib::Server svr_;
    std::thread server_thread_;
    std::atomic<bool> running_{false};
};

} // namespace server
