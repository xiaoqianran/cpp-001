#include <SQLiteCpp/SQLiteCpp.h>
#include <cassert>
#include <iostream>

int main() {
    // 直接使用 SQLiteCpp 真实库 + in-memory
    SQLite::Database db(":memory:", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT)");
    db.exec("INSERT INTO test (name) VALUES (\"hello\")");

    SQLite::Statement query(db, "SELECT name FROM test WHERE id = 1");
    query.executeStep();

    std::string name = query.getColumn(0);
    assert(name == "hello");

    std::cout << "sqlite test passed\n";
    return 0;
}
