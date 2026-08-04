#pragma once

#include <string>
#include <string_view>
#include <toml++/toml.h>
#include <vector>

#include "common/Result.hpp"

namespace common {

/**
 * 最小配置加载器（toml++）。
 * - 支持字符串 / 文件加载
 * - 值语义友好，RAII
 * - get_* 返回 Result
 */
class Config {
public:
    Config() = default;
    explicit Config(std::string_view toml_content);

    /** 从 toml 字符串加载（解析失败返回 error） */
    static Result<Config, std::string> load_string(std::string_view toml_content);

    /** 从文件路径加载（filesystem + toml++） */
    static Result<Config, std::string> load_file(const std::string& path);

    /**
     * 按候选路径依次尝试 load_file，全部失败则用 fallback 字符串。
     * 便于从项目根或 build 目录启动。
     */
    static Config load_with_fallback(const std::vector<std::string>& candidates,
                                     std::string_view fallback_toml);

    Result<std::string, std::string> get_string(std::string_view key) const;
    Result<int, std::string> get_int(std::string_view key) const;

private:
    explicit Config(toml::table tbl);

    toml::table tbl_;
};

} // namespace common
