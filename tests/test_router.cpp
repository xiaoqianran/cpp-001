#include "router/Router.hpp"
#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    router::Router r;
    controller::Controller ctrl;

    // 普通路由
    r.add_route("GET", "/status", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    // 带参数路由 (skeleton)
    bool param_called = false;
    r.add_route("GET", "/users/{id}", [&param_called](const httplib::Request&, httplib::Response& res) {
        param_called = true;
        res.set_content("user param route", "text/plain");
    });

    httplib::Request req;
    httplib::Response res;

    // 测试普通
    bool d1 = r.dispatch("GET", "/status", req, res);
    assert(d1);
    assert(res.body == "service layer OK");

    // 测试参数路由
    bool d2 = r.dispatch("GET", "/users/42", req, res);
    assert(d2);
    assert(param_called);
    assert(res.body == "user param route");

    // 不匹配
    bool d3 = r.dispatch("GET", "/unknown", req, res);
    assert(!d3);

    std::cout << "router params skeleton test passed\n";
    return 0;
}
