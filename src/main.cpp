#include <iostream>

// NOTE: main.cpp follows strict rule - ONLY startup, config assembly, logs, server lifecycle.
// NO business logic, NO controllers, NO services here.

int main() {
    std::cout << "cpp-001 C++ backend starting..." << std::endl;

    // Stub: assemble config (later from src/config/)
    std::cout << "[config] Configuration loaded (stub)." << std::endl;

    // Stub: initialize logging (later spdlog etc)
    std::cout << "[log] Logger initialized (stub)." << std::endl;

    // Stub: server lifecycle (later in src/server/)
    std::cout << "[server] Server starting (stub)..." << std::endl;
    std::cout << "[server] Listening on 0.0.0.0:8080 (stub)." << std::endl;

    // In real: server.run() would block here.
    std::cout << "cpp-001 ready (stub - bootstrap)." << std::endl;

    return 0;
}
