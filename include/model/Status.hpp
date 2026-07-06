#pragma once

#include <string>

namespace model {

/**
 * 最小 model 层骨架。
 * 代表领域数据 / DTO。
 * 纯数据 + 简单验证。
 */
struct Status {
    std::string message;
    bool healthy = true;

    Status() = default;
    explicit Status(std::string msg, bool ok = true) 
        : message(std::move(msg)), healthy(ok) {}

    bool is_healthy() const { return healthy; }
};

} // namespace model
