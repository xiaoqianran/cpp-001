#include "common/AtomicCounter.hpp"

#include <cassert>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    common::AtomicCounter counter;

    const int num_threads = 4;
    const int increments_per_thread = 1000;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([&counter, increments_per_thread]() {
            for (int j = 0; j < increments_per_thread; ++j) {
                counter.increment();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    std::uint64_t expected = static_cast<std::uint64_t>(num_threads) * increments_per_thread;
    assert(counter.get() == expected);

    std::cout << "atomic counter test passed (count=" << counter.get() << ")\n";
    return 0;
}
