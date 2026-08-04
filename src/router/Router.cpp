#include "router/Router.hpp"

#include <sstream>

namespace router {

namespace {

std::vector<std::string> split_segments(const std::string& path) {
    std::vector<std::string> parts;
    std::size_t i = 0;
    if (!path.empty() && path[0] == '/') {
        i = 1;
    }
    while (i < path.size()) {
        auto j = path.find('/', i);
        if (j == std::string::npos) {
            parts.push_back(path.substr(i));
            break;
        }
        parts.push_back(path.substr(i, j - i));
        i = j + 1;
    }
    return parts;
}

bool is_param_segment(const std::string& seg, std::string& name_out) {
    if (seg.size() >= 3 && seg.front() == '{' && seg.back() == '}') {
        name_out = seg.substr(1, seg.size() - 2);
        return !name_out.empty();
    }
    return false;
}

} // namespace

std::string to_httplib_pattern(const std::string& path) {
    std::string out;
    out.reserve(path.size());
    for (std::size_t i = 0; i < path.size(); ++i) {
        if (path[i] == '{') {
            out.push_back(':');
            ++i;
            while (i < path.size() && path[i] != '}') {
                out.push_back(path[i]);
                ++i;
            }
            // skip '}'
        } else {
            out.push_back(path[i]);
        }
    }
    return out;
}

bool match_and_extract(const std::string& pattern,
                       const std::string& path,
                       PathParams& params) {
    params.clear();
    const auto pp = split_segments(pattern);
    const auto rp = split_segments(path);
    if (pp.size() != rp.size()) {
        return false;
    }
    for (std::size_t i = 0; i < pp.size(); ++i) {
        std::string name;
        if (is_param_segment(pp[i], name)) {
            params.emplace(name, rp[i]);
        } else if (pp[i] != rp[i]) {
            return false;
        }
    }
    return true;
}

void Router::add_route(const std::string& method, const std::string& path, Handler handler) {
    routes_.push_back({method, path, std::move(handler)});
}

bool Router::dispatch(const std::string& method,
                      const std::string& path,
                      httplib::Request& req,
                      httplib::Response& res) const {
    for (const auto& r : routes_) {
        PathParams params;
        if (r.method == method && match_and_extract(r.path, path, params)) {
            req.path = path;
            req.path_params = std::move(params);
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
        const auto pattern = to_httplib_pattern(r.path);
        if (r.method == "GET") {
            svr.Get(pattern, r.handler);
        } else if (r.method == "POST") {
            svr.Post(pattern, r.handler);
        } else if (r.method == "PUT") {
            svr.Put(pattern, r.handler);
        } else if (r.method == "DELETE") {
            svr.Delete(pattern, r.handler);
        }
    }
}

} // namespace router
