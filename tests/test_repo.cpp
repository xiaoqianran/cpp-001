#include "repository/KvRepository.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    repository::KvRepository repo(":memory:");

    auto set_res = repo.set("app", "cpp-001");
    assert(set_res.is_ok());
    assert(set_res.value() == true);

    auto get_res = repo.get("app");
    assert(get_res.is_ok());
    assert(get_res.value() == "cpp-001");

    auto missing = repo.get("nope");
    assert(missing.is_err());

    // 覆盖写
    assert(repo.set("app", "updated").is_ok());
    assert(repo.get("app").value() == "updated");

    std::cout << "repo KvRepository test passed (value=" << get_res.value() << ")\n";
    return 0;
}
