#include "router/Router.hpp"

namespace router {

static bool matches(const std::string& route_path, const std::string& req_path) {
    if (route_path.find('{') == std::string::npos) {
        return route_path == req_path;
    }
    // skeleton support for {param}
    auto pos = route_path.find('{');
    std::string prefix = route_path.substr(0, pos);
    if (req_path.size() < prefix.size() || req_path.substr(0, prefix.size()) != prefix) {
        return false;
    }
    auto pos2 = route_path.find('}', pos);
    if (pos2 == std::string::npos) {
        return true;
    }
    std::string suffix = route_path.substr(pos2 + 1);
    if (suffix.empty()) {
        return true;
    }
    if (req_path.size() >= suffix.size() && req_path.substr(req_path.size() - suffix.size()) == suffix) {
        return true;
    }
    return false;
}

void Router::add_route(const std::string& method, const std::string& path, Handler handler) {
    routes_.push_back({method, path, std::move(handler)});
}

bool Router::dispatch(const std::string& method, const std::string& path, const httplib::Request& req, httplib::Response& res) const {
    for (const auto& r : routes_) {
        if (r.method == method && matches(r.path, path)) {
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
        } else if (r.method == "PUT") {
            svr.Put(r.path, r.handler);
        } else if (r.method == "DELETE") {
            svr.Delete(r.path, r.handler);
        }
    }
}

} // namespace router
