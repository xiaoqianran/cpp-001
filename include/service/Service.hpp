#pragma once

#include "common/Result.hpp"
#include "model/Status.hpp"
#include "repository/KvRepository.hpp"

#include <memory>
#include <string>
#include <vector>

namespace service {

/**
 * service 层：业务逻辑。
 * 默认持有内存 KvRepository；也可注入外部 repository。
 */
class Service {
public:
    Service() : repo_(std::make_shared<repository::KvRepository>(":memory:")) {
        (void)repo_->set("status_message", "service layer OK");
    }

    explicit Service(std::shared_ptr<repository::KvRepository> repo)
        : repo_(std::move(repo)) {}

    virtual ~Service() = default;

    common::Result<model::Status, std::string> get_status() const {
        auto r = repo_->get("status_message");
        if (r.is_ok()) {
            return model::Status(r.value());
        }
        return {false, r.error()};
    }

    common::Result<std::string, std::string> get_kv(const std::string& key) const {
        if (key.empty()) {
            return {false, "empty key"};
        }
        return repo_->get(key);
    }

    common::Result<bool, std::string> put_kv(const std::string& key, const std::string& value) {
        if (key.empty()) {
            return {false, "empty key"};
        }
        return repo_->set(key, value);
    }

    common::Result<bool, std::string> delete_kv(const std::string& key) {
        if (key.empty()) {
            return {false, "empty key"};
        }
        return repo_->del(key);
    }

    common::Result<std::vector<std::string>, std::string> list_keys() const {
        return repo_->keys();
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
