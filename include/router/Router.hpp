#pragma once

#include <httplib.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace router {

/**
 * Router 层：管理路由表。
 * 支持 {param} 路径参数；dispatch 会写入 req.path_params。
 * apply_to 将 {param} 转为 httplib 的 :param 模式。
 */
using Handler = std::function<void(const httplib::Request&, httplib::Response&)>;
using PathParams = std::unordered_map<std::string, std::string>;

struct Route {
    std::string method;
    std::string path;
    Handler handler;
};

/** 将 /users/{id} 转为 httplib 可识别的 /users/:id */
std::string to_httplib_pattern(const std::string& path);

/** 段匹配 + 提取路径参数；成功返回 true 并填充 params */
bool match_and_extract(const std::string& pattern,
                       const std::string& path,
                       PathParams& params);

class Router {
public:
    void add_route(const std::string& method, const std::string& path, Handler handler);

    // 注意：req 非 const，以便写入 path_params
    bool dispatch(const std::string& method,
                  const std::string& path,
                  httplib::Request& req,
                  httplib::Response& res) const;

    void apply_to(httplib::Server& svr) const;

private:
    std::vector<Route> routes_;
};

} // namespace router
