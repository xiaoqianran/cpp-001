#include "router/Router.hpp"
#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    router::Router r;
    controller::Controller ctrl;

    r.add_route("GET", "/status", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    // POST 示例
    r.add_route("POST", "/echo", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content("POST received: " + req.body, "text/plain");
    });

    // param
    r.add_route("GET", "/users/{id}", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("param route", "text/plain");
    });

    httplib::Request req;
    httplib::Response res;

    bool d1 = r.dispatch("GET", "/status", req, res);
    assert(d1);

    // POST
    httplib::Request preq;
    preq.body = "hello";
    bool d2 = r.dispatch("POST", "/echo", preq, res);
    assert(d2);
    assert(res.body == "POST received: hello");

    bool d3 = r.dispatch("GET", "/users/99", req, res);
    assert(d3);

    std::cout << "router http methods test passed\n";
    return 0;
}
