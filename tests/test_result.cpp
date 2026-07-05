#include "common/Result.hpp"
#include "common/Status.hpp"

#include <cassert>
#include <string>
#include <iostream>

int main() {
    // 直接测试真实 Result
    common::Result<int, std::string> ok(42);
    assert(ok.is_ok());
    assert(!ok.is_err());
    assert(ok.value() == 42);

    common::Result<int, std::string> err("failure");
    assert(!err.is_ok());
    assert(err.is_err());
    assert(err.error() == "failure");

    std::cout << "result test passed\n";
    return 0;
}
