#include "common/Logger.hpp"

#include <iomanip>
#include <sstream>

namespace common {

Logger::Logger(std::ostream& output_stream)
    : out(output_stream) {}

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
    out << '[' << format_timestamp() << "] "
        << '[' << level_to_string(level) << "] "
        << message << '\n';
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
