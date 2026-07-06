#include "router/Router.hpp"
#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    router::Router r;
    controller::Controller ctrl;

    // 注册 controller handler 到 router
    r.add_route("GET", "/status", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    httplib::Request req;
    httplib::Response res;

    bool dispatched = r.dispatch("GET", "/status", req, res);
    assert(dispatched);
    assert(res.body == "service layer OK");
    assert(res.status == 200);

    bool not_found = r.dispatch("GET", "/unknown", req, res);
    assert(!not_found);

    std::cout << "router + controller integration test passed\n";
    return 0;
}
