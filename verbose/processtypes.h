#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/process/processtypes.h"

// Verbose from ProcState version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 3> ProcStatePairs = {{
    {"IDLE", static_cast<int>(::syspilot::ProcState::IDLE)},
    {"WORKING", static_cast<int>(::syspilot::ProcState::WORKING)},
    {"STOPPED", static_cast<int>(::syspilot::ProcState::STOPPED)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ProcState value) {
    for (auto const& item : ::syspilot::ProcStatePairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end ProcState.

// Manual part
