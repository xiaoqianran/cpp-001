#include <httplib.h>
#include <cassert>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

int main() {
    httplib::Server svr;

    svr.Get("/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Hello from cpp-001", "text/plain");
    });

    // Run server in background thread
    std::thread server_thread([&svr]() {
        svr.listen("127.0.0.1", 18080);
    });

    // Give server time to start
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    // Client request
    httplib::Client cli("http://127.0.0.1:18080");
    auto res = cli.Get("/hello");

    svr.stop();
    server_thread.join();

    assert(res && res->status == 200);
    assert(res->body == "Hello from cpp-001");

    std::cout << "httpserver test passed (status=" << res->status << ")\n";
    return 0;
}
