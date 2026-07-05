#include "common/Config.hpp"
#include "common/Status.hpp"

#include <cassert>
#include <iostream>

int main() {
    std::string toml = R"(
        server_name = "cpp-001"
        port = 8080
    )";

    common::Config cfg(toml);

    auto name_res = cfg.get_string("server_name");
    assert(name_res.is_ok());
    assert(name_res.value() == "cpp-001");

    auto port_res = cfg.get_int("port");
    assert(port_res.is_ok());
    assert(port_res.value() == 8080);

    // unknown key
    auto unknown = cfg.get_string("missing");
    assert(unknown.is_err());
    assert(unknown.error().find("missing") != std::string::npos);

    std::cout << "config test passed\n";
    return 0;
}
