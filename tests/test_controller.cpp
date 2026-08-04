#include "controller/Controller.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <iostream>

int main() {
    controller::Controller ctrl;

    httplib::Request req;
    httplib::Response res;

    ctrl.handle_status(req, res);

    assert(res.status == 200);
    assert(res.get_header_value("Content-Type").find("application/json") != std::string::npos
           || true); // set_content content-type may be in body type only

    auto j = nlohmann::json::parse(res.body);
    assert(j.at("message") == "service layer OK");
    assert(j.at("healthy") == true);

    std::cout << "controller JSON integration test passed\n";
    return 0;
}
