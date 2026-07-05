#pragma once

#include <string>
#include <string_view>
#include <optional>
#include <toml++/toml.h>

namespace common {

/**
 * 最小配置加载器（toml++ 接入）。
 * - 支持从字符串解析（便于测试）。
 * - 值语义友好，RAII。
 * - 提供类型安全 get。
 */
class Config {
public:
    explicit Config(std::string_view toml_content);

    std::optional<std::string> get_string(std::string_view key) const;
    std::optional<int> get_int(std::string_view key) const;

private:
    toml::table tbl_;
};

} // namespace common
