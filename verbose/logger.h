#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/logging/logger.h"

// Manual part
#include <mutex>

namespace syspilot {

inline std::ostream& operator<<(std::ostream& os, const Logger& logger)
{
    std::lock_guard lock(logger.mutex_);

    for(const auto& log : logger.logs_) {
        os << log << '\n';
    }

    return os;
}

} // namespace syspilot
