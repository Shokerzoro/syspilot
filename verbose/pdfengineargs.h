#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#include "../src/args/pdfengineargs.h"

// Verbose from PdfEngineCommands version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> PdfEngineCommandsPairs = {{
    {"snapshot", static_cast<int>(::syspilot::PdfEngineCommands::snapshot)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::PdfEngineCommands value) {
    for (auto const& item : ::syspilot::PdfEngineCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end PdfEngineCommands.

// Verbose from PdfEngineArgs version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> PdfEngineArgsPairs = {{
    {"output", static_cast<int>(::syspilot::PdfEngineArgs::output)},
}};
} // namespace syspilot

inline std::ostream& operator<<(std::ostream& os, ::syspilot::PdfEngineArgs value) {
    for (auto const& item : ::syspilot::PdfEngineArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return os << item.first;
        }
    }
    return os << static_cast<int>(value);
}
// Verbose end PdfEngineArgs.

// Manual part
#include "../src/args/clioptions.h"

namespace syspilot {

template<>
struct CLIEnumTraits<PdfEngineCommands> {
    static std::string to_string(PdfEngineCommands value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<PdfEngineCommands> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<PdfEngineCommands>(PdfEngineCommandsPairs, value);
    }
};

template<>
struct CLIEnumTraits<PdfEngineArgs> {
    static std::string to_string(PdfEngineArgs value)
    {
        std::ostringstream stream;
        stream << value;
        return stream.str();
    }

    static std::optional<PdfEngineArgs> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<PdfEngineArgs>(PdfEngineArgsPairs, value);
    }
};

using PdfEngineCLIOptions = CLIOptions<PdfEngineCommands, PdfEngineArgs>;

} // namespace syspilot
