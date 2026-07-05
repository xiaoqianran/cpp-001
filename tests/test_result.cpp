#include "common/Result.hpp"
#include <cassert>
#include <string>
#include <iostream>

int main() {
    common::Result<int, std::string> ok(42);
    assert(ok.is_ok());
    assert(ok.value_or(0) == 42);

    common::Result<int, std::string> err(false, std::string("failure"));
    assert(err.is_err());
    assert(err.value_or(99) == 99);

    // map demo
    auto doubled = ok.map([](int v) { return v * 2; });
    assert(doubled.is_ok());
    assert(doubled.value() == 84);

    // map on error keeps error
    auto err_mapped = err.map([](int v) { return v * 2; });
    assert(err_mapped.is_err());
    assert(err_mapped.error() == "failure");

    std::cout << "result chain test passed\n";
    return 0;
}
