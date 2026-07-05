#include "common/Status.hpp"

namespace common {

Status::Status(bool ok, std::string message)
    : ok_(ok), message_(std::move(message)) {}

Status Status::Ok() {
    return Status(true);
}

Status Status::Error(std::string message) {
    return Status(false, std::move(message));
}

} // namespace common
