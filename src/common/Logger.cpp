#include "common/Logger.hpp"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/ostream_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fmt/format.h>
#include <iomanip>
#include <sstream>
#include <memory>

namespace common {

namespace {
spdlog::level::level_enum to_spdlog_level(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return spdlog::level::debug;
        case LogLevel::Info:  return spdlog::level::info;
        case LogLevel::Warn:  return spdlog::level::warn;
        case LogLevel::Error: return spdlog::level::err;
        default:              return spdlog::level::info;
    }
}
} // anonymous

Logger::Logger(std::ostream& output_stream)
    : out(output_stream), logger_(nullptr) {
    if (&output_stream == &std::cout) {
        logger_ = spdlog::stdout_color_mt("console");
        logger_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%L] %v");
    } else {
        // keep old fmt path for test injection compatibility
    }
    if (logger_) {
        logger_->set_level(spdlog::level::debug);
    }
}

std::string Logger::format_timestamp() const {
    using namespace std::chrono;
    auto now = system_clock::now();
    auto time = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tm_buf{};
#if defined(_WIN32)
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif

    std::ostringstream ts;
    ts << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S")
       << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ts.str();
}

void Logger::log(LogLevel level, std::string_view message) {
    if (logger_) {
        logger_->log(to_spdlog_level(level), "{}", message);
    } else {
        out << fmt::format("[{}] [{}] {}\n", format_timestamp(), level_to_string(level), message);
    }
}

std::string_view level_to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info:  return "INFO";
        case LogLevel::Warn:  return "WARN";
        case LogLevel::Error: return "ERROR";
        default:              return "UNKNOWN";
    }
}

} // namespace common
