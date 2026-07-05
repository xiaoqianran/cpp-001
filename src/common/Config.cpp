#include "common/Config.hpp"

namespace common {

Config::Config(std::string_view toml_content) {
    tbl_ = toml::parse(toml_content);
}

std::optional<std::string> Config::get_string(std::string_view key) const {
    if (auto val = tbl_[key].value<std::string>()) {
        return *val;
    }
    return std::nullopt;
}

std::optional<int> Config::get_int(std::string_view key) const {
    if (auto val = tbl_[key].value<int>()) {
        return *val;
    }
    return std::nullopt;
}

} // namespace common
