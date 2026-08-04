#pragma once

#include <SQLiteCpp/SQLiteCpp.h>

#include "common/Result.hpp"

#include <memory>
#include <string>

namespace repository {

/**
 * 最小 repository 层：基于 SQLiteCpp 的 KV 存储。
 * - 值语义友好（通过 shared_ptr / 移动持有 Database）
 * - 返回 Result 做显式错误处理
 * - 无业务逻辑
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

    // Database 不可拷贝
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

private:
    // get() 为 const，但 SQLite Statement 需要非 const Database 引用
    mutable std::unique_ptr<SQLite::Database> db_;
};

} // namespace repository
