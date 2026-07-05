#include <yaml-cpp/yaml.h>
#include <cassert>
#include <iostream>
#include <string>

int main() {
    std::string y = R"(
server:
  name: cpp-001
  port: 9000
)";
    YAML::Node node = YAML::Load(y);
    assert(node["server"]["name"].as<std::string>() == "cpp-001");
    assert(node["server"]["port"].as<int>() == 9000);
    std::cout << "yaml test passed\n";
    return 0;
}
