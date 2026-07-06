#pragma once

#include <httplib.h>
#include <string>

#include "service/Service.hpp"

namespace controller {

/**
 * 最小 controller 层。
 * 现在集成 service 层进行业务调用。
 */
class Controller {
public:
    virtual ~Controller() = default;

    // 处理 GET /status —— 调用 service 获取业务结果
    virtual void handle_status(const httplib::Request& req, httplib::Response& res) {
        service::Service svc;
        auto result = svc.get_status();
        if (result.is_ok()) {
            res.set_content(result.value(), "text/plain");
            res.status = 200;
        } else {
            res.set_content("error", "text/plain");
            res.status = 500;
        }
    }
};

} // namespace controller
