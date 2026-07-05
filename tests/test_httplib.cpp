#include <httplib.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // 最小 demo：启动临时 server + client 请求
    httplib::Server svr;
    svr.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello from cpp-001", "text/plain");
    });

    // 在后台启动
    std::thread server_thread([&svr]() {
        svr.listen("127.0.0.1", 18080);
    });

    // 等待 server 启动
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    httplib::Client cli("127.0.0.1", 18080);
    auto res = cli.Get("/hello");

    assert(res && res->status == 200);
    assert(res->body == "Hello from cpp-001");

    svr.stop();
    server_thread.join();

    std::cout << "httplib test passed\n";
    return 0;
}
