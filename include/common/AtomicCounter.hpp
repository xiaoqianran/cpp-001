#pragma once

#include <atomic>
#include <cstdint>

namespace common {

/**
 * 基于 std::atomic 的无锁计数器（并发练习）。
 */
class AtomicCounter {
public:
    void increment() {
        count_.fetch_add(1, std::memory_order_relaxed);
    }

    std::uint64_t get() const {
        return count_.load(std::memory_order_relaxed);
    }

private:
    std::atomic<std::uint64_t> count_{0};
};

} // namespace common
