#pragma once

#include <httplib.h>
#include <string>

#include "service/Service.hpp"

namespace controller {

/**
 * 最小 controller 层。
 * 调用 service 获取 model::Status。
 */
class Controller {
public:
    virtual ~Controller() = default;

    virtual void handle_status(const httplib::Request& req, httplib::Response& res) {
        service::Service svc;
        auto result = svc.get_status();
        if (result.is_ok()) {
            res.set_content(result.value().message, "text/plain");
            res.status = 200;
        } else {
            res.set_content("error", "text/plain");
            res.status = 500;
        }
    }
};

} // namespace controller
