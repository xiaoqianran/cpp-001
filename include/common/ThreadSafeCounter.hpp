#pragma once

#include <mutex>
#include <cstdint>

namespace common {

/**
 * 最小线程安全计数器（标准库并发练习）。
 * 使用 std::mutex 保护。
 */
class ThreadSafeCounter {
public:
    void increment() {
        std::lock_guard<std::mutex> lock(mutex_);
        ++count_;
    }

    std::uint64_t get() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return count_;
    }

private:
    mutable std::mutex mutex_;
    std::uint64_t count_ = 0;
};

} // namespace common
