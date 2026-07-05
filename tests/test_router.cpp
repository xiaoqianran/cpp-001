#include "router/Router.hpp"
#include <cassert>
#include <iostream>

int main() {
    router::Router r;
    bool called = false;

    r.add_route("GET", "/api/status", [&called]() { called = true; });

    bool dispatched = r.dispatch("GET", "/api/status");
    assert(dispatched);
    assert(called);

    bool not_found = r.dispatch("GET", "/unknown");
    assert(!not_found);

    std::cout << "router layer test passed\n";
    return 0;
}
