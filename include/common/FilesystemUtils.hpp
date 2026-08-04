#pragma once

#include "common/Result.hpp"

#include <string>

namespace common {

/**
 * 标准库专项：std::filesystem 最小工具。
 * 值语义返回，无裸指针。
 */
std::string current_working_directory();

/** 读取整个文本文件；失败返回 Result error */
Result<std::string, std::string> read_text_file(const std::string& path);

/** 判断路径是否存在且为常规文件 */
bool file_exists(const std::string& path);

} // namespace common
