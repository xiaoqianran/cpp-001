#include "common/FilesystemUtils.hpp"

#include <iostream>
#include <cassert>
#include <string>

// 直接驱动真实 common::FilesystemUtils 实现。

int main() {
    std::string cwd = common::current_working_directory();

    assert(!cwd.empty());
    // 基本合理性检查：路径应包含分隔符或至少有长度
    assert(cwd.length() > 1);

    std::cout << "test_filesystem: current_working_directory() = " << cwd << std::endl;
    std::cout << "test_filesystem: all assertions passed (real std::filesystem impl exercised)." << std::endl;
    return 0;
}
