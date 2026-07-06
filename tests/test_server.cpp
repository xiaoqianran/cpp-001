#include "server/Server.hpp"
#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    server::Server app;
    controller::Controller ctrl;

    // 使用完整分层链路：server -> router -> controller -> service -> model
    app.route("GET", "/hello", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    app.listen("127.0.0.1", 18081);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    httplib::Client cli("http://127.0.0.1:18081");
    auto res = cli.Get("/hello");

    app.stop();

    assert(res && res->status == 200);
    assert(res->body == "service layer OK");

    std::cout << "server e2e full chain test passed\n";
    return 0;
}
