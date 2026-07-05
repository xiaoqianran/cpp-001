#include "common/FilesystemUtils.hpp"

#include <filesystem>

namespace common {

std::string current_working_directory() {
    return std::filesystem::current_path().string();
}

} // namespace common
