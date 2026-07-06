#pragma once

#include <httplib.h>
#include <string>

namespace controller {

/**
 * 最小 controller 层骨架。
 * 职责：将 HTTP 请求转换为业务输入，调用 service，转换响应。
 * 当前为演示骨架。
 */
class Controller {
public:
    virtual ~Controller() = default;

    // 示例：处理 GET /status
    virtual void handle_status(const httplib::Request& req, httplib::Response& res) {
        res.set_content("controller layer OK", "text/plain");
    }

    // 未来可添加更多 action
};

} // namespace controller
