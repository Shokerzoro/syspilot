#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/args/uniterargs.h"

// Verbose from UniterCommands version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> UniterCommandsPairs = {{
    {"run", static_cast<int>(::syspilot::UniterCommands::run)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::UniterCommands value) {
    for (auto const& item : ::syspilot::UniterCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end UniterCommands.

// Verbose from UniterArgs version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 4> UniterArgsPairs = {{
    {"servertype", static_cast<int>(::syspilot::UniterArgs::servertype)},
    {"serverip", static_cast<int>(::syspilot::UniterArgs::serverip)},
    {"updated", static_cast<int>(::syspilot::UniterArgs::updated)},
    {"updatefailed", static_cast<int>(::syspilot::UniterArgs::updatefailed)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::UniterArgs value) {
    for (auto const& item : ::syspilot::UniterArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end UniterArgs.

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

using UniterCLIOptions = CLIOptions<UniterCommands, UniterArgs>;

} // namespace syspilot
