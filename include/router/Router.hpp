#pragma once

#include <httplib.h>
#include <functional>
#include <string>
#include <vector>

namespace router {

/**
 * Router 层：管理路由表。
 * 支持与 Server 集成。
 */
using Handler = std::function<void(const httplib::Request&, httplib::Response&)>;

struct Route {
    std::string method;
    std::string path;
    Handler handler;
};

class Router {
public:
    void add_route(const std::string& method, const std::string& path, Handler handler);

    bool dispatch(const std::string& method, const std::string& path, const httplib::Request& req, httplib::Response& res) const;

    // 集成：将所有路由应用到 httplib::Server
    void apply_to(httplib::Server& svr) const;

private:
    std::vector<Route> routes_;
};

} // namespace router
