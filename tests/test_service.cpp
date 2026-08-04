#include "service/Service.hpp"
#include <cassert>
#include <iostream>

int main() {
    service::Service svc;

    auto res = svc.get_status();
    assert(res.is_ok());
    assert(res.value().message == "service layer OK");
    assert(res.value().is_healthy());

    assert(svc.echo("hello") == "echo: hello");
    assert(svc.echo("") == "echo: (empty)");

    assert(svc.put_kv("k1", "v1").is_ok());
    assert(svc.get_kv("k1").value() == "v1");
    assert(svc.get_kv("missing").is_err());
    assert(svc.delete_kv("k1").value() == true);
    assert(svc.get_kv("k1").is_err());

    assert(svc.put_kv("x", "1").is_ok());
    assert(svc.put_kv("y", "2").is_ok());
    assert(svc.list_keys().value().size() == 3); // status_message + x + y

    std::cout << "service + kv test passed\n";
    return 0;
}
