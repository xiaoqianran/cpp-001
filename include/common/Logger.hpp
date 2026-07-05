#pragma once

#include <ostream>
#include <string>
#include <string_view>
#include <iostream>
#include <memory>

namespace spdlog { class logger; }

namespace common {

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error
};

class Logger {
public:
    explicit Logger(std::ostream& output_stream = std::cout);

    void log(LogLevel level, std::string_view message);

private:
    std::string format_timestamp() const;
    std::ostream& out;
    std::shared_ptr<spdlog::logger> logger_;
};

std::string_view level_to_string(LogLevel level);

} // namespace common
