#include "common/Config.hpp"
#include "common/Status.hpp"

#include <cassert>
#include <iostream>

int main() {
    // 直接驱动真实 Config + toml++
    std::string toml = R"(
        server_name = "cpp-001"
        port = 8080
    )";

    common::Config cfg(toml);

    auto name = cfg.get_string("server_name");
    assert(name.has_value());
    assert(*name == "cpp-001");

    auto port = cfg.get_int("port");
    assert(port.has_value());
    assert(*port == 8080);

    // unknown key
    auto unknown = cfg.get_string("missing");
    assert(!unknown.has_value());

    std::cout << "config test passed\n";
    return 0;
}
