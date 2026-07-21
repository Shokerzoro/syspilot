#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#ifdef QT_CORE_LIB
#include <QDebug>
#endif

#include "../src/args/uniterargs.h"

// Verbose from UniterCommands version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> UniterCommandsPairs = {{
    {"run", static_cast<int>(::syspilot::UniterCommands::run)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::UniterCommands value) {
    for (auto const& item : ::syspilot::UniterCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::UniterCommands value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::UniterCommands value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end UniterCommands.

// Verbose from UniterArgs version 3.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 2> UniterArgsPairs = {{
    {"servertype", static_cast<int>(::syspilot::UniterArgs::servertype)},
    {"serverip", static_cast<int>(::syspilot::UniterArgs::serverip)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::UniterArgs value) {
    for (auto const& item : ::syspilot::UniterArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::UniterArgs value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::UniterArgs value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end UniterArgs.

// Verbose from UniterFlags version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 3> UniterFlagsPairs = {{
    {"updated", static_cast<int>(::syspilot::UniterFlags::updated)},
    {"updatefailed", static_cast<int>(::syspilot::UniterFlags::updatefailed)},
    {"local", static_cast<int>(::syspilot::UniterFlags::local)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::UniterFlags value) {
    for (auto const& item : ::syspilot::UniterFlagsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::UniterFlags value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::UniterFlags value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end UniterFlags.

// Manual part
#include "../src/args/clioptions.h"

namespace syspilot {

template<>
struct CLIEnumTraits<UniterCommands> {
    static std::string to_string(UniterCommands value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<UniterCommands> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<UniterCommands>(UniterCommandsPairs, value);
    }
};

template<>
struct CLIEnumTraits<UniterArgs> {
    static std::string to_string(UniterArgs value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<UniterArgs> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<UniterArgs>(UniterArgsPairs, value);
    }
};

template<>
struct CLIEnumTraits<UniterFlags> {
    static std::string to_string(UniterFlags value)
    {
        return ::to_string(value);
    }

    static std::optional<UniterFlags> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<UniterFlags>(UniterFlagsPairs, value);
    }
};

using UniterCLIOptions = CLIOptions<UniterCommands, UniterArgs, UniterFlags>;

} // namespace syspilot
