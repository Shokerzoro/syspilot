#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#ifdef QT_CORE_LIB
#include <QDebug>
#endif

#include "../src/args/elevatorargs.h"

// Verbose from ElevatorCommands version 3.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 3> ElevatorCommandsPairs = {{
    {"update", static_cast<int>(::syspilot::ElevatorCommands::update)},
    {"restartapp", static_cast<int>(::syspilot::ElevatorCommands::restartapp)},
    {"uninstall", static_cast<int>(::syspilot::ElevatorCommands::uninstall)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::ElevatorCommands value) {
    for (auto const& item : ::syspilot::ElevatorCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ElevatorCommands value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::ElevatorCommands value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end ElevatorCommands.

// Verbose from ElevatorArgs version 3.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 4> ElevatorArgsPairs = {{
    {"approot", static_cast<int>(::syspilot::ElevatorArgs::approot)},
    {"updates", static_cast<int>(::syspilot::ElevatorArgs::updates)},
    {"newversion", static_cast<int>(::syspilot::ElevatorArgs::newversion)},
    {"apppid", static_cast<int>(::syspilot::ElevatorArgs::apppid)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::ElevatorArgs value) {
    for (auto const& item : ::syspilot::ElevatorArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ElevatorArgs value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::ElevatorArgs value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end ElevatorArgs.

// Verbose from ElevatorFlags version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> ElevatorFlagsPairs = {{
    {"prepared", static_cast<int>(::syspilot::ElevatorFlags::prepared)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::ElevatorFlags value) {
    for (auto const& item : ::syspilot::ElevatorFlagsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::ElevatorFlags value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::ElevatorFlags value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end ElevatorFlags.

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

template<>
struct CLIEnumTraits<ElevatorFlags> {
    static std::string to_string(ElevatorFlags value)
    {
        return ::to_string(value);
    }

    static std::optional<ElevatorFlags> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<ElevatorFlags>(ElevatorFlagsPairs, value);
    }
};

using ElevatorCLIOptions = CLIOptions<ElevatorCommands, ElevatorArgs, ElevatorFlags>;

} // namespace syspilot
