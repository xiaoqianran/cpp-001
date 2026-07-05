#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>

namespace router {

struct Route {
    std::string method;
    std::string path;
    std::function<void()> handler;  // 简化 handler
};

class Router {
public:
    void add_route(const std::string& method, const std::string& path, std::function<void()> handler);
    bool dispatch(const std::string& method, const std::string& path) const;

private:
    std::vector<Route> routes_;
};

} // namespace router
