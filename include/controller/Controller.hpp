#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <string>

#include "service/Service.hpp"

namespace controller {

/**
 * controller 层：请求/响应 I/O 转换（无业务逻辑）。
 * 调用 service，将 model 序列化为 JSON。
 */
class Controller {
public:
    virtual ~Controller() = default;

    virtual void handle_status(const httplib::Request& /*req*/, httplib::Response& res) {
        service::Service svc;
        auto result = svc.get_status();
        if (result.is_ok()) {
            const auto& st = result.value();
            nlohmann::json body = {
                {"message", st.message},
                {"healthy", st.healthy},
            };
            res.set_content(body.dump(), "application/json");
            res.status = 200;
        } else {
            nlohmann::json body = {
                {"error", result.error()},
            };
            res.set_content(body.dump(), "application/json");
            res.status = 500;
        }
    }
};

} // namespace controller
