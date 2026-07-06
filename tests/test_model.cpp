#include "model/Status.hpp"
#include <cassert>
#include <iostream>

int main() {
    model::Status s1;
    assert(s1.message.empty());
    assert(s1.is_healthy());

    model::Status s2("all good");
    assert(s2.message == "all good");
    assert(s2.healthy);

    model::Status s3("down", false);
    assert(!s3.is_healthy());

    std::cout << "model layer test passed\n";
    return 0;
}
