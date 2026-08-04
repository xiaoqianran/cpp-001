#pragma once

#include <httplib.h>
#include <nlohmann/json.hpp>
#include <memory>
#include <string>

#include "service/Service.hpp"

namespace controller {

/**
 * controller 层：请求/响应 I/O 转换（无业务逻辑）。
 * 可注入共享 Service，保证跨请求状态一致（KV 等）。
 */
class Controller {
public:
    Controller() : svc_(std::make_shared<service::Service>()) {}

    explicit Controller(std::shared_ptr<service::Service> svc) : svc_(std::move(svc)) {}

    virtual ~Controller() = default;

    virtual void handle_status(const httplib::Request& /*req*/, httplib::Response& res) {
        auto result = svc_->get_status();
        if (result.is_ok()) {
            const auto& st = result.value();
            nlohmann::json body = {
                {"message", st.message},
                {"healthy", st.healthy},
            };
            res.set_content(body.dump(), "application/json");
            res.status = 200;
        } else {
            write_error(res, 500, result.error());
        }
    }

    /** GET /kv/{key} — path_params["key"] */
    virtual void handle_get_kv(const httplib::Request& req, httplib::Response& res) {
        const auto key = path_param(req, "key");
        if (key.empty()) {
            write_error(res, 400, "missing path param: key");
            return;
        }
        auto result = svc_->get_kv(key);
        if (result.is_ok()) {
            nlohmann::json body = {{"key", key}, {"value", result.value()}};
            res.set_content(body.dump(), "application/json");
            res.status = 200;
        } else {
            write_error(res, 404, result.error());
        }
    }

    /** PUT /kv/{key} body: raw text 或 {"value":"..."} */
    virtual void handle_put_kv(const httplib::Request& req, httplib::Response& res) {
        const auto key = path_param(req, "key");
        if (key.empty()) {
            write_error(res, 400, "missing path param: key");
            return;
        }
        std::string value = req.body;
        if (!req.body.empty() && req.body.front() == '{') {
            auto j = nlohmann::json::parse(req.body, nullptr, false);
            if (!j.is_discarded() && j.contains("value")) {
                if (j["value"].is_string()) {
                    value = j["value"].get<std::string>();
                } else {
                    value = j["value"].dump();
                }
            }
        }
        auto result = svc_->put_kv(key, value);
        if (result.is_ok()) {
            nlohmann::json body = {{"key", key}, {"value", value}, {"ok", true}};
            res.set_content(body.dump(), "application/json");
            res.status = 200;
        } else {
            write_error(res, 500, result.error());
        }
    }

    /** DELETE /kv/{key} */
    virtual void handle_delete_kv(const httplib::Request& req, httplib::Response& res) {
        const auto key = path_param(req, "key");
        if (key.empty()) {
            write_error(res, 400, "missing path param: key");
            return;
        }
        auto result = svc_->delete_kv(key);
        if (result.is_ok()) {
            nlohmann::json body = {{"key", key}, {"deleted", result.value()}};
            res.set_content(body.dump(), "application/json");
            res.status = result.value() ? 200 : 404;
            if (!result.value()) {
                body["error"] = "key not found: " + key;
                res.set_content(body.dump(), "application/json");
            }
        } else {
            write_error(res, 500, result.error());
        }
    }

    /** GET /kv — 列出全部 key */
    virtual void handle_list_kv(const httplib::Request& /*req*/, httplib::Response& res) {
        auto result = svc_->list_keys();
        if (result.is_ok()) {
            nlohmann::json body = {{"keys", result.value()}};
            res.set_content(body.dump(), "application/json");
            res.status = 200;
        } else {
            write_error(res, 500, result.error());
        }
    }

    /** POST /echo — body 原样回显（经 service） */
    virtual void handle_echo(const httplib::Request& req, httplib::Response& res) {
        nlohmann::json body = {{"echo", svc_->echo(req.body)}};
        res.set_content(body.dump(), "application/json");
        res.status = 200;
    }

    service::Service& service() { return *svc_; }

private:
    static std::string path_param(const httplib::Request& req, const std::string& name) {
        auto it = req.path_params.find(name);
        if (it == req.path_params.end()) {
            return {};
        }
        return it->second;
    }

    static void write_error(httplib::Response& res, int status, const std::string& msg) {
        nlohmann::json body = {{"error", msg}};
        res.set_content(body.dump(), "application/json");
        res.status = status;
    }

    std::shared_ptr<service::Service> svc_;
};

} // namespace controller
