#include "common/Config.hpp"
#include "common/FilesystemUtils.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

int main() {
    // 字符串加载
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

    auto unknown = cfg.get_string("missing");
    assert(unknown.is_err());

    // load_string 错误处理
    auto bad = common::Config::load_string("not = [ valid");
    assert(bad.is_err());

    // 写临时文件再 load_file
    const std::string tmp = "test_config_tmp.toml";
    {
        std::ofstream out(tmp);
        out << "app_name = \"from-file\"\nport = 18080\n";
    }
    assert(common::file_exists(tmp));
    auto file_cfg = common::Config::load_file(tmp);
    assert(file_cfg.is_ok());
    assert(file_cfg.value().get_string("app_name").value() == "from-file");
    assert(file_cfg.value().get_int("port").value() == 18080);

    auto missing_file = common::Config::load_file("definitely_missing_config.toml");
    assert(missing_file.is_err());

    // load_with_fallback：错误路径走 fallback
    auto fb = common::Config::load_with_fallback(
        {"definitely_missing_config.toml"},
        R"(app_name = "fallback"
port = 9
)");
    assert(fb.get_string("app_name").value() == "fallback");
    assert(fb.get_int("port").value() == 9);

    std::remove(tmp.c_str());

    std::cout << "config file + string test passed\n";
    return 0;
}
