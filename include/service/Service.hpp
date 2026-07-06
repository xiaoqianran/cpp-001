#pragma once

#include "common/Result.hpp"
#include <string>

namespace service {

/**
 * 最小 service 层骨架。
 * 职责：业务逻辑封装（纯业务，无 I/O、无框架细节）。
 * 当前为演示骨架，使用 Result 返回示例。
 */
class Service {
public:
    virtual ~Service() = default;

    // 示例业务逻辑：获取状态
    common::Result<std::string, std::string> get_status() const {
        return common::Result<std::string, std::string>("service layer OK");
    }

    // 示例：简单 echo 业务逻辑
    std::string echo(const std::string& input) const {
        if (input.empty()) {
            return "echo: (empty)";
        }
        return "echo: " + input;
    }
};

} // namespace service
