#include "common/Config.hpp"
#include "common/FilesystemUtils.hpp"

namespace common {

Config::Config(std::string_view toml_content) {
    tbl_ = toml::parse(toml_content);
}

Config::Config(toml::table tbl) : tbl_(std::move(tbl)) {}

Result<Config, std::string> Config::load_string(std::string_view toml_content) {
    try {
        return Config(toml::parse(toml_content));
    } catch (const std::exception& e) {
        return {false, std::string("toml parse error: ") + e.what()};
    }
}

Result<Config, std::string> Config::load_file(const std::string& path) {
    auto content = read_text_file(path);
    if (content.is_err()) {
        return {false, content.error()};
    }
    try {
        return Config(toml::parse(content.value()));
    } catch (const std::exception& e) {
        return {false, std::string("toml parse error in ") + path + ": " + e.what()};
    }
}

Config Config::load_with_fallback(const std::vector<std::string>& candidates,
                                  std::string_view fallback_toml) {
    for (const auto& path : candidates) {
        auto res = load_file(path);
        if (res.is_ok()) {
            return res.value();
        }
    }
    auto fb = load_string(fallback_toml);
    if (fb.is_ok()) {
        return fb.value();
    }
    // 最后兜底：空表
    return Config(toml::table{});
}

Result<std::string, std::string> Config::get_string(std::string_view key) const {
    if (auto val = tbl_[key].value<std::string>()) {
        return Result<std::string, std::string>(*val);
    }
    return Result<std::string, std::string>(false, std::string("key not found: ") + std::string(key));
}

Result<int, std::string> Config::get_int(std::string_view key) const {
    if (auto val = tbl_[key].value<int64_t>()) {
        return Result<int, std::string>(static_cast<int>(*val));
    }
    if (auto val = tbl_[key].value<int>()) {
        return Result<int, std::string>(*val);
    }
    return Result<int, std::string>(false, std::string("key not found or wrong type: ") + std::string(key));
}

} // namespace common
