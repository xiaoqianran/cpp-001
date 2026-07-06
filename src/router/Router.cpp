#include "router/Router.hpp"

namespace router {

void Router::add_route(const std::string& method, const std::string& path, Handler handler) {
    routes_.push_back({method, path, std::move(handler)});
}

bool Router::dispatch(const std::string& method, const std::string& path, const httplib::Request& req, httplib::Response& res) const {
    for (const auto& r : routes_) {
        if (r.method == method && r.path == path) {
            if (r.handler) {
                r.handler(req, res);
            }
            return true;
        }
    }
    return false;
}

void Router::apply_to(httplib::Server& svr) const {
    for (const auto& r : routes_) {
        if (r.method == "GET") {
            svr.Get(r.path, r.handler);
        } else if (r.method == "POST") {
            svr.Post(r.path, r.handler);
        }
        // 可扩展
    }
}

} // namespace router
