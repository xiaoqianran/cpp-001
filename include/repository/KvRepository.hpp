#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "common/Result.hpp"

#include <memory>
#include <string>
#include <vector>

namespace repository {

/**
 * 最小 repository 层：基于 SQLiteCpp 的 KV 存储。
 */
class KvRepository {
public:
    explicit KvRepository(const std::string& path = ":memory:")
        : db_(std::make_unique<SQLite::Database>(
              path, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE)) {
        db_->exec(
            "CREATE TABLE IF NOT EXISTS kv ("
            "  key TEXT PRIMARY KEY,"
            "  value TEXT NOT NULL"
            ");");
    }

    KvRepository(const KvRepository&) = delete;
    KvRepository& operator=(const KvRepository&) = delete;
    KvRepository(KvRepository&&) noexcept = default;
    KvRepository& operator=(KvRepository&&) noexcept = default;

    common::Result<std::string, std::string> get(const std::string& key) const {
        try {
            SQLite::Statement query(*db_, "SELECT value FROM kv WHERE key = ?");
            query.bind(1, key);
            if (query.executeStep()) {
                return std::string(query.getColumn(0).getText());
            }
            return {false, "key not found: " + key};
        } catch (const std::exception& e) {
            return {false, std::string(e.what())};
        }
    }

    common::Result<bool, std::string> set(const std::string& key, const std::string& value) {
        try {
            SQLite::Statement stmt(
                *db_, "INSERT OR REPLACE INTO kv (key, value) VALUES (?, ?)");
            stmt.bind(1, key);
            stmt.bind(2, value);
            stmt.exec();
            return true;
        } catch (const std::exception& e) {
            return {false, std::string(e.what())};
        }
    }

    /** 删除 key；不存在时 ok=true 且 value=false */
    common::Result<bool, std::string> del(const std::string& key) {
        try {
            SQLite::Statement stmt(*db_, "DELETE FROM kv WHERE key = ?");
            stmt.bind(1, key);
            stmt.exec();
            return db_->getChanges() > 0;
        } catch (const std::exception& e) {
            return {false, std::string(e.what())};
        }
    }

    common::Result<std::vector<std::string>, std::string> keys() const {
        try {
            std::vector<std::string> out;
            SQLite::Statement query(*db_, "SELECT key FROM kv ORDER BY key");
            while (query.executeStep()) {
                out.emplace_back(query.getColumn(0).getText());
            }
            return out;
        } catch (const std::exception& e) {
            return {false, std::string(e.what())};
        }
    }

private:
    mutable std::unique_ptr<SQLite::Database> db_;
};

} // namespace repository
