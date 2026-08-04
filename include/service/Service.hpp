#pragma once

#include "common/Result.hpp"
#include "model/Status.hpp"
#include "repository/KvRepository.hpp"

#include <memory>
#include <string>

namespace service {

/**
 * service 层：业务逻辑。
 * 默认持有内存 KvRepository；也可注入外部 repository。
 */
class Service {
public:
    Service() : repo_(std::make_shared<repository::KvRepository>(":memory:")) {
        // 种子数据，保证 /health 链路可演示
        (void)repo_->set("status_message", "service layer OK");
    }

    explicit Service(std::shared_ptr<repository::KvRepository> repo)
        : repo_(std::move(repo)) {}

    virtual ~Service() = default;

    // 从 repository 读取状态消息，封装为 model::Status
    common::Result<model::Status, std::string> get_status() const {
        auto r = repo_->get("status_message");
        if (r.is_ok()) {
            return model::Status(r.value());
        }
        // 缺 key 时给出明确错误，仍保持 Result 语义
        return {false, r.error()};
    }

    std::string echo(const std::string& input) const {
        if (input.empty()) {
            return "echo: (empty)";
        }
        return "echo: " + input;
    }

    repository::KvRepository& repo() { return *repo_; }
    const repository::KvRepository& repo() const { return *repo_; }

private:
    std::shared_ptr<repository::KvRepository> repo_;
};

} // namespace service
