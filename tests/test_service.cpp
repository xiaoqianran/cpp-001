#include "service/Service.hpp"
#include <cassert>
#include <iostream>

int main() {
    service::Service svc;

    // 测试 status 返回 Result
    auto res = svc.get_status();
    assert(res.is_ok());
    assert(res.value() == "service layer OK");

    // 测试 echo 业务逻辑
    std::string echoed = svc.echo("hello");
    assert(echoed == "echo: hello");

    std::string empty_echo = svc.echo("");
    assert(empty_echo == "echo: (empty)");

    std::cout << "service layer test passed\n";
    return 0;
}
