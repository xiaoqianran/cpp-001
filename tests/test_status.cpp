#include "common/Status.hpp"
#include <iostream>
#include <cassert>
#include <utility>

// Test directly drives the shipped common/Status implementation (real symbols, fresh state each time).
// No mocks, no reimplementation inside test.

int main() {
    // Test Ok path
    auto ok = common::Status::Ok();
    assert(ok.ok());
    assert(!ok.is_error());
    assert(ok.message().empty());

    // Test Error path
    auto err = common::Status::Error("something went wrong");
    assert(!err.ok());
    assert(err.is_error());
    assert(err.message() == "something went wrong");

    // Value semantics: copy
    auto err2 = err;
    assert(err2.message() == err.message());
    assert(err2.is_error());

    // Value semantics: move
    auto err3 = std::move(err2);
    assert(err3.message() == "something went wrong");
    // after move, err2 is in valid but unspecified state - we don't rely on it

    std::cout << "test_status: all assertions passed (real implementation exercised)." << std::endl;
    return 0;
}
