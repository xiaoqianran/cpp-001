#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    controller::Controller ctrl;

    httplib::Request req;
    httplib::Response res;

    ctrl.handle_status(req, res);

    assert(res.body == "controller layer OK");
    std::cout << "controller layer test passed\n";
    return 0;
}
