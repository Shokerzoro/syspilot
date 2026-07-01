#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/args/elevatorargs.h"

// Verbose from ElevatorCommands version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 2> ElevatorCommandsPairs = {{
    {"update", static_cast<int>(::syspilot::ElevatorCommands::update)},
    {"uninstall", static_cast<int>(::syspilot::ElevatorCommands::uninstall)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ElevatorCommands value) {
    for (auto const& item : ::syspilot::ElevatorCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end ElevatorCommands.

// Verbose from ElevatorArgs version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 5> ElevatorArgsPairs = {{
    {"approot", static_cast<int>(::syspilot::ElevatorArgs::approot)},
    {"updates", static_cast<int>(::syspilot::ElevatorArgs::updates)},
    {"newversion", static_cast<int>(::syspilot::ElevatorArgs::newversion)},
    {"prepared", static_cast<int>(::syspilot::ElevatorArgs::prepared)},
    {"apppid", static_cast<int>(::syspilot::ElevatorArgs::apppid)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ElevatorArgs value) {
    for (auto const& item : ::syspilot::ElevatorArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end ElevatorArgs.

// Manual part
#include "../src/args/clioptions.h"

namespace syspilot {

template<>
struct CLIEnumTraits<ElevatorCommands> {
    static std::string to_string(ElevatorCommands value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<ElevatorCommands> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<ElevatorCommands>(ElevatorCommandsPairs, value);
    }
};

template<>
struct CLIEnumTraits<ElevatorArgs> {
    static std::string to_string(ElevatorArgs value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<ElevatorArgs> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<ElevatorArgs>(ElevatorArgsPairs, value);
    }
};

using ElevatorCLIOptions = CLIOptions<ElevatorCommands, ElevatorArgs>;

} // namespace syspilot
