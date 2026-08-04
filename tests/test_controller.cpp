#include "controller/Controller.hpp"
#include "service/Service.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <iostream>
#include <memory>

int main() {
    auto svc = std::make_shared<service::Service>();
    controller::Controller ctrl(svc);

    httplib::Request req;
    httplib::Response res;
    ctrl.handle_status(req, res);
    assert(res.status == 200);
    auto j = nlohmann::json::parse(res.body);
    assert(j.at("message") == "service layer OK");
    assert(j.at("healthy") == true);

    httplib::Request put_req;
    put_req.path_params["key"] = "t";
    put_req.body = R"({"value":"42"})";
    httplib::Response put_res;
    ctrl.handle_put_kv(put_req, put_res);
    assert(put_res.status == 200);

    httplib::Request get_req;
    get_req.path_params["key"] = "t";
    httplib::Response get_res;
    ctrl.handle_get_kv(get_req, get_res);
    assert(get_res.status == 200);
    assert(nlohmann::json::parse(get_res.body).at("value") == "42");

    httplib::Request list_req;
    httplib::Response list_res;
    ctrl.handle_list_kv(list_req, list_res);
    assert(list_res.status == 200);
    auto keys = nlohmann::json::parse(list_res.body).at("keys");
    assert(keys.is_array());

    std::cout << "controller JSON + KV test passed\n";
    return 0;
}
