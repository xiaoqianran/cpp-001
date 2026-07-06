#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    controller::Controller ctrl;

    httplib::Request req;
    httplib::Response res;

    ctrl.handle_status(req, res);

    // 现在验证 controller 调用了 service
    assert(res.body == "service layer OK");
    assert(res.status == 200);

    std::cout << "controller + service integration test passed\n";
    return 0;
}
