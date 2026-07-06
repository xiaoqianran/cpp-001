#pragma once

#include <httplib.h>
#include <functional>
#include <string>
#include <thread>
#include <atomic>

#include "router/Router.hpp"

namespace server {

/**
 * 最小 server 层封装（基于 cpp-httplib）。
 * 现在持有 Router 层进行路由管理。
 */
class Server {
public:
    using Handler = router::Handler;  // 复用 Router 的 Handler

    Server();
    ~Server();

    void route(const std::string& method, const std::string& pattern, Handler handler);
    void listen(const std::string& host, int port);
    void stop();

private:
    httplib::Server svr_;
    router::Router router_;
    std::thread server_thread_;
    std::atomic<bool> running_{false};
};

} // namespace server
