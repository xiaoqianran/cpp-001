#pragma once

#include <utility>
#include <string>

namespace common {

/**
 * 最小 Result<T, E>（C++17）。
 * 支持 is_ok/is_err/value/error/value_or
 * 提供简单链式辅助（value_or + map 基础版）
 */
template <typename T, typename E>
class Result {
public:
    Result(T value) : ok_(true), value_(std::move(value)) {}
    Result(bool /*tag*/, E error) : ok_(false), error_(std::move(error)) {}

    bool is_ok() const { return ok_; }
    bool is_err() const { return !ok_; }

    const T& value() const { return value_; }
    const E& error() const { return error_; }

    // 实用链式方法
    T value_or(const T& default_v) const {
        return is_ok() ? value_ : default_v;
    }

    template <typename F>
    auto map(F&& f) const -> Result<decltype(f(std::declval<const T&>())), E> {
        using NewT = decltype(f(std::declval<const T&>()));
        if (is_ok()) {
            return Result<NewT, E>(f(value_));
        }
        return Result<NewT, E>(false, error_);
    }

private:
    bool ok_;
    T value_;
    E error_;
};

} // namespace common
