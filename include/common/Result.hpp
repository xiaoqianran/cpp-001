#pragma once

#include <utility>
#include <string>

namespace common {

/**
 * 最小 Result<T, E>（C++17 实现，避免 variant 同类型歧义）。
 * - 值语义。
 * - 支持 is_ok / is_err / value / error。
 */
template <typename T, typename E>
class Result {
public:
    // OK 构造
    Result(T value) : ok_(true), value_(std::move(value)) {}

    // ERR 构造
    Result(bool /*tag*/, E error) : ok_(false), error_(std::move(error)) {}

    bool is_ok() const { return ok_; }
    bool is_err() const { return !ok_; }

    const T& value() const { return value_; }
    const E& error() const { return error_; }

private:
    bool ok_;
    T value_;
    E error_;
};

} // namespace common
