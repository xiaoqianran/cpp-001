#pragma once

#include <variant>
#include <utility>

namespace common {

/**
 * 最小 Result<T, E>（C++17 变体实现，类似 std::expected）。
 * - 值语义。
 * - 支持 is_ok / is_err / value / error。
 * - 用于错误处理，替代裸异常或 bool。
 */
template <typename T, typename E>
class Result {
public:
    Result(T value) : data_(std::move(value)) {}
    Result(E error) : data_(std::move(error)) {}

    bool is_ok() const { return std::holds_alternative<T>(data_); }
    bool is_err() const { return std::holds_alternative<E>(data_); }

    const T& value() const { return std::get<T>(data_); }
    const E& error() const { return std::get<E>(data_); }

private:
    std::variant<T, E> data_;
};

} // namespace common
