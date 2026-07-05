#include "router/Router.hpp"

namespace router {

void Router::add_route(const std::string& method, const std::string& path, std::function<void()> handler) {
    routes_.push_back({method, path, std::move(handler)});
}

bool Router::dispatch(const std::string& method, const std::string& path) const {
    for (const auto& r : routes_) {
        if (r.method == method && r.path == path) {
            if (r.handler) r.handler();
            return true;
        }
    }
    return false;
}

} // namespace router
