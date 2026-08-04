#include "server/Server.hpp"
#include "controller/Controller.hpp"
#include "service/Service.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

int main() {
    auto svc = std::make_shared<service::Service>();
    controller::Controller ctrl(svc);
    server::Server app;

    app.route("GET", "/hello", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });
    app.route("GET", "/kv/{key}", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_get_kv(req, res);
    });
    app.route("PUT", "/kv/{key}", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_put_kv(req, res);
    });
    app.route("DELETE", "/kv/{key}", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_delete_kv(req, res);
    });
    app.route("POST", "/echo", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_echo(req, res);
    });

    app.listen("127.0.0.1", 18081);
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    httplib::Client cli("http://127.0.0.1:18081");

    auto res = cli.Get("/hello");
    assert(res && res->status == 200);
    auto j = nlohmann::json::parse(res->body);
    assert(j.at("message") == "service layer OK");

    auto put = cli.Put("/kv/demo", R"({"value":"hello-kv"})", "application/json");
    assert(put && put->status == 200);

    auto get = cli.Get("/kv/demo");
    assert(get && get->status == 200);
    auto gj = nlohmann::json::parse(get->body);
    assert(gj.at("key") == "demo");
    assert(gj.at("value") == "hello-kv");

    auto echo = cli.Post("/echo", "ping", "text/plain");
    assert(echo && echo->status == 200);
    assert(echo->body.find("echo: ping") != std::string::npos);

    auto del = cli.Delete("/kv/demo");
    assert(del && del->status == 200);
    auto missing = cli.Get("/kv/demo");
    assert(missing && missing->status == 404);

    app.stop();

    std::cout << "server e2e full chain JSON + KV test passed\n";
    return 0;
}
