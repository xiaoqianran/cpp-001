#include "repository/KvRepository.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    repository::KvRepository repo(":memory:");

    assert(repo.set("app", "cpp-001").is_ok());
    assert(repo.get("app").value() == "cpp-001");
    assert(repo.get("nope").is_err());

    assert(repo.set("app", "updated").is_ok());
    assert(repo.get("app").value() == "updated");

    auto del = repo.del("app");
    assert(del.is_ok() && del.value() == true);
    assert(repo.get("app").is_err());
    auto del2 = repo.del("app");
    assert(del2.is_ok() && del2.value() == false);

    assert(repo.set("a", "1").is_ok());
    assert(repo.set("b", "2").is_ok());
    auto keys = repo.keys();
    assert(keys.is_ok());
    assert(keys.value().size() == 2);

    std::cout << "repo KvRepository CRUD test passed\n";
    return 0;
}
