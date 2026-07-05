#pragma once

#include <string>
#include <string_view>
#include <toml++/toml.h>

#include "common/Result.hpp"

namespace common {

/**
 * 最小配置加载器（toml++ 接入）。
 * - 支持从字符串解析（便于测试）。
 * - 值语义友好，RAII。
 * - 提供类型安全 get，返回 Result 以显式错误处理。
 */
class Config {
public:
    explicit Config(std::string_view toml_content);

    Result<std::string, std::string> get_string(std::string_view key) const;
    Result<int, std::string> get_int(std::string_view key) const;

private:
    toml::table tbl_;
};

} // namespace common
