#include "common/FilesystemUtils.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace common {

namespace fs = std::filesystem;

std::string current_working_directory() {
    return fs::current_path().string();
}

bool file_exists(const std::string& path) {
    std::error_code ec;
    return fs::is_regular_file(path, ec);
}

Result<std::string, std::string> read_text_file(const std::string& path) {
    if (!file_exists(path)) {
        return {false, "file not found: " + path};
    }
    std::ifstream in(path);
    if (!in) {
        return {false, "cannot open file: " + path};
    }
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

} // namespace common
