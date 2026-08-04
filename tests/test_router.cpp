#include "router/Router.hpp"
#include "controller/Controller.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>
#include <memory>

int main() {
    // pattern 转换
    assert(router::to_httplib_pattern("/users/{id}") == "/users/:id");
    assert(router::to_httplib_pattern("/kv/{key}/meta") == "/kv/:key/meta");

    router::PathParams params;
    assert(router::match_and_extract("/users/{id}", "/users/99", params));
    assert(params.at("id") == "99");
    assert(!router::match_and_extract("/users/{id}", "/users/99/extra", params));
    assert(!router::match_and_extract("/users/{id}", "/posts/99", params));

    router::Router r;
    auto svc = std::make_shared<service::Service>();
    controller::Controller ctrl(svc);

    r.add_route("GET", "/status", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_status(req, res);
    });

    r.add_route("POST", "/echo", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_echo(req, res);
    });

    r.add_route("GET", "/users/{id}", [](const httplib::Request& req, httplib::Response& res) {
        res.set_content(req.path_params.at("id"), "text/plain");
    });

    r.add_route("GET", "/kv/{key}", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_get_kv(req, res);
    });
    r.add_route("PUT", "/kv/{key}", [&ctrl](const httplib::Request& req, httplib::Response& res) {
        ctrl.handle_put_kv(req, res);
    });

    httplib::Request req;
    httplib::Response res;

    assert(r.dispatch("GET", "/status", req, res));

    httplib::Request preq;
    preq.body = "hello";
    httplib::Response pres;
    assert(r.dispatch("POST", "/echo", preq, pres));
    assert(pres.body.find("echo: hello") != std::string::npos);

    httplib::Request ureq;
    httplib::Response ures;
    assert(r.dispatch("GET", "/users/99", ureq, ures));
    assert(ures.body == "99");
    assert(ureq.path_params.at("id") == "99");

    // KV put + get via path params
    httplib::Request put_req;
    put_req.body = R"({"value":"v1"})";
    httplib::Response put_res;
    assert(r.dispatch("PUT", "/kv/foo", put_req, put_res));
    assert(put_res.status == 200);

    httplib::Request get_req;
    httplib::Response get_res;
    assert(r.dispatch("GET", "/kv/foo", get_req, get_res));
    assert(get_res.status == 200);
    assert(get_res.body.find("v1") != std::string::npos);
    assert(get_req.path_params.at("key") == "foo");

    std::cout << "router path-params + methods test passed\n";
    return 0;
}
