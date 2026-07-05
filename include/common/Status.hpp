#pragma once

#include <string>

namespace common {

/**
 * Status represents a value-semantic result for operations,
 * following RAII and modern C++ value semantics (no raw pointers,
 * no ownership issues). Used for error handling without exceptions
 * in core paths.
 */
class Status {
public:
    Status() = default;

    /// Success status with no message.
    static Status Ok();

    /// Error status with message.
    static Status Error(std::string message);

    bool ok() const noexcept { return ok_; }
    bool is_error() const noexcept { return !ok_; }

    const std::string& message() const noexcept { return message_; }

    // Rule: value semantics - copies and moves are cheap and correct by default.

private:
    explicit Status(bool ok, std::string message = "");

    bool ok_ = true;
    std::string message_;
};

} // namespace common
