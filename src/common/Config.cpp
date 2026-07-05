#include "common/Config.hpp"

namespace common {

Config::Config(std::string_view toml_content) {
    tbl_ = toml::parse(toml_content);
}

Result<std::string, std::string> Config::get_string(std::string_view key) const {
    if (auto val = tbl_[key].value<std::string>()) {
        return Result<std::string, std::string>(*val);
    }
    return Result<std::string, std::string>(false, std::string("key not found: ") + std::string(key));
}

Result<int, std::string> Config::get_int(std::string_view key) const {
    if (auto val = tbl_[key].value<int>()) {
        return Result<int, std::string>(*val);
    }
    return Result<int, std::string>(false, std::string("key not found or wrong type: ") + std::string(key));
}

} // namespace common
