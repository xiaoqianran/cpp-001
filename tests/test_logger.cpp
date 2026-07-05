#include "common/Logger.hpp"
#include "common/Status.hpp"  // 确保可以同时使用 common 下其他组件

#include <sstream>
#include <string>
#include <cassert>
#include <iostream>

// 直接驱动真实 common::Logger 实现。
// 使用 std::ostringstream 注入输出，便于断言，无副作用。
// 验证：时间戳格式、级别字符串、消息内容。

int main() {
    std::ostringstream oss;
    common::Logger logger(oss);

    // 测试 Info
    logger.log(common::LogLevel::Info, "Server starting up");
    std::string out1 = oss.str();
    assert(out1.find("INFO") != std::string::npos);
    assert(out1.find("Server starting up") != std::string::npos);
    assert(out1.find("20") != std::string::npos); // 年份或日期数字

    // 测试 Warn + 清空
    oss.str(""); oss.clear();
    logger.log(common::LogLevel::Warn, "Configuration missing default");
    std::string out2 = oss.str();
    assert(out2.find("WARN") != std::string::npos);
    assert(out2.find("Configuration missing default") != std::string::npos);

    // 测试 Error
    oss.str(""); oss.clear();
    logger.log(common::LogLevel::Error, "Failed to bind port");
    std::string out3 = oss.str();
    assert(out3.find("[ERROR]") != std::string::npos);

    // 测试 Debug
    oss.str(""); oss.clear();
    logger.log(common::LogLevel::Debug, "Detailed trace info");
    assert(oss.str().find("[DEBUG]") != std::string::npos);

    // 组合：Status + Logger （演示 common 层内协同，真实链接）
    auto status = common::Status::Error("test error");
    oss.str(""); oss.clear();
    if (status.is_error()) {
        logger.log(common::LogLevel::Error, status.message());
    }
    assert(oss.str().find("test error") != std::string::npos);

    std::cout << "test_logger: all assertions passed (real Logger + Status exercised)." << std::endl;
    return 0;
}
