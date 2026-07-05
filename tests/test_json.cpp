#include <nlohmann/json.hpp>
#include <cassert>
#include <iostream>
#include <string>

using json = nlohmann::json;

int main() {
    // 直接驱动 nlohmann/json 真实库
    json j;
    j["name"] = "cpp-001";
    j["port"] = 8080;
    j["tags"] = {"backend", "learning"};

    std::string dumped = j.dump(2);
    assert(dumped.find("cpp-001") != std::string::npos);
    assert(dumped.find("8080") != std::string::npos);

    // parse back
    json j2 = json::parse(dumped);
    assert(j2["name"] == "cpp-001");
    assert(j2["port"] == 8080);
    assert(j2["tags"].size() == 2);

    std::cout << "json test passed\n";
    return 0;
}
