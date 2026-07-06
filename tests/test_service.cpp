#include "service/Service.hpp"
#include <cassert>
#include <iostream>

int main() {
    service::Service svc;

    auto res = svc.get_status();
    assert(res.is_ok());
    assert(res.value().message == "service layer OK");
    assert(res.value().is_healthy());

    std::string echoed = svc.echo("hello");
    assert(echoed == "echo: hello");

    std::string empty_echo = svc.echo("");
    assert(empty_echo == "echo: (empty)");

    std::cout << "service + model test passed\n";
    return 0;
}
