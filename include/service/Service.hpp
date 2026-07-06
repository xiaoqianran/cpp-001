#pragma once

#include "common/Result.hpp"
#include "model/Status.hpp"
#include <string>

namespace service {

/**
 * 最小 service 层骨架。
 * 现在使用 model::Status 返回业务数据。
 */
class Service {
public:
    virtual ~Service() = default;

    // 返回 model 层对象
    common::Result<model::Status, std::string> get_status() const {
        return model::Status("service layer OK");
    }

    std::string echo(const std::string& input) const {
        if (input.empty()) {
            return "echo: (empty)";
        }
        return "echo: " + input;
    }
};

} // namespace service
