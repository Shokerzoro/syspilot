#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/paths/pathtypes.h"

// Verbose from DirType version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 6> DirTypePairs = {{
    {"Binaries", static_cast<int>(::syspilot::DirType::Binaries)},
    {"AppRoot", static_cast<int>(::syspilot::DirType::AppRoot)},
    {"Database", static_cast<int>(::syspilot::DirType::Database)},
    {"FileCache", static_cast<int>(::syspilot::DirType::FileCache)},
    {"Updates", static_cast<int>(::syspilot::DirType::Updates)},
    {"LocalMinIO", static_cast<int>(::syspilot::DirType::LocalMinIO)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::DirType value) {
    for (auto const& item : ::syspilot::DirTypePairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end DirType.

// Verbose from BinType version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 3> BinTypePairs = {{
    {"App", static_cast<int>(::syspilot::BinType::App)},
    {"Elevator", static_cast<int>(::syspilot::BinType::Elevator)},
    {"PdfEngine", static_cast<int>(::syspilot::BinType::PdfEngine)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::BinType value) {
    for (auto const& item : ::syspilot::BinTypePairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end BinType.

// Manual part
