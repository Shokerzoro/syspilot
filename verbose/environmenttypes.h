#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/environment/environmenttypes.h"

// Verbose from EnvType version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 15> EnvTypePairs = {{
    {"Version", static_cast<int>(::syspilot::EnvType::Version)},
    {"AppRoot", static_cast<int>(::syspilot::EnvType::AppRoot)},
    {"Binaries", static_cast<int>(::syspilot::EnvType::Binaries)},
    {"Database", static_cast<int>(::syspilot::EnvType::Database)},
    {"FileCache", static_cast<int>(::syspilot::EnvType::FileCache)},
    {"Updates", static_cast<int>(::syspilot::EnvType::Updates)},
    {"LocalMinIO", static_cast<int>(::syspilot::EnvType::LocalMinIO)},
    {"AppExecutable", static_cast<int>(::syspilot::EnvType::AppExecutable)},
    {"ElevatorExecutable", static_cast<int>(::syspilot::EnvType::ElevatorExecutable)},
    {"PdfEngineExecutable", static_cast<int>(::syspilot::EnvType::PdfEngineExecutable)},
    {"AppPid", static_cast<int>(::syspilot::EnvType::AppPid)},
    {"ElevatorPid", static_cast<int>(::syspilot::EnvType::ElevatorPid)},
    {"PdfEnginePid", static_cast<int>(::syspilot::EnvType::PdfEnginePid)},
    {"ServerType", static_cast<int>(::syspilot::EnvType::ServerType)},
    {"ServerIp", static_cast<int>(::syspilot::EnvType::ServerIp)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::EnvType value) {
    for (auto const& item : ::syspilot::EnvTypePairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end EnvType.

// Manual part
