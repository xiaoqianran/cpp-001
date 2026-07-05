#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <chrono>
#include <iostream>  // for default argument std::cout in declaration

namespace common {

/**
 * LogLevel: 日志级别枚举。值语义，简单可复制。
 */
enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error
};

/**
 * 最小日志封装（bootstrap 后标准库专项）。
 * - 使用 std::chrono 获取时间戳（C++17）。
 * - 接受 std::ostream& 引用（非拥有，便于测试用 stringstream 注入）。
 * - 值语义友好（enum 是，类本身为轻量引用持有）。
 * - 遵循硬规则：无裸指针、无业务逻辑、无大型依赖。
 * - 后续可替换为 spdlog 等。
 */
class Logger {
public:
    explicit Logger(std::ostream& output_stream = std::cout);

    /// 记录一条日志，自动添加时间戳和级别前缀。
    void log(LogLevel level, std::string_view message);

private:
    std::ostream& out;
    std::string format_timestamp() const;
};

/// 辅助：级别转字符串（值语义）。
std::string_view level_to_string(LogLevel level);

} // namespace common
