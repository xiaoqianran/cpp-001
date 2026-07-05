#include "server/Server.hpp"
#include <httplib.h>
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    server::Server app;

    app.route("GET", "/hello", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Server layer OK", "text/plain");
    });

    app.listen("127.0.0.1", 18081);

    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    httplib::Client cli("http://127.0.0.1:18081");
    auto res = cli.Get("/hello");

    app.stop();

    assert(res && res->status == 200);
    assert(res->body == "Server layer OK");

    std::cout << "server layer test passed\n";
    return 0;
}
