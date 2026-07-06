#include "router/Router.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>

int main() {
    router::Router r;
    bool called = false;

    r.add_route("GET", "/api/status", [&called](const httplib::Request&, httplib::Response&) {
        called = true;
    });

    httplib::Request req;
    httplib::Response res;
    bool dispatched = r.dispatch("GET", "/api/status", req, res);
    assert(dispatched);
    assert(called);

    bool not_found = r.dispatch("GET", "/unknown", req, res);
    assert(!not_found);

    std::cout << "router layer test passed\n";
    return 0;
}
