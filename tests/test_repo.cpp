#include <SQLiteCpp/SQLiteCpp.h>
#include "common/Result.hpp"

#include <cassert>
#include <iostream>
#include <string>

int main() {
    try {
        SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

        db.exec("CREATE TABLE IF NOT EXISTS kv (key TEXT PRIMARY KEY, value TEXT);");
        db.exec("INSERT OR REPLACE INTO kv (key, value) VALUES ('app', 'cpp-001');");

        SQLite::Statement query(db, "SELECT value FROM kv WHERE key = ?");
        query.bind(1, "app");

        if (query.executeStep()) {
            std::string val = query.getColumn(0);
            assert(val == "cpp-001");
            std::cout << "repo example passed (value=" << val << ")\n";
            return 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "SQLite error: " << e.what() << std::endl;
    }
    assert(false && "repo test failed");
    return 1;
}
