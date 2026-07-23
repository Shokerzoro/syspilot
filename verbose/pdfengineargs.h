#pragma once

#include <array>
#include <ostream>
#include <string>
#include <utility>

#ifdef QT_CORE_LIB
#include <QDebug>
#endif

#include "../src/args/pdfengineargs.h"

// Verbose from PdfEngineCommands version 2.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 1> PdfEngineCommandsPairs = {{
    {"snapshot", static_cast<int>(::syspilot::PdfEngineCommands::snapshot)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::PdfEngineCommands value) {
    for (auto const& item : ::syspilot::PdfEngineCommandsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::PdfEngineCommands value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::PdfEngineCommands value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end PdfEngineCommands.

// Verbose from PdfEngineArgs version 3.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 2> PdfEngineArgsPairs = {{
    {"input", static_cast<int>(::syspilot::PdfEngineArgs::input)},
    {"output", static_cast<int>(::syspilot::PdfEngineArgs::output)},
}};
} // namespace syspilot

inline std::string to_string(::syspilot::PdfEngineArgs value) {
    for (auto const& item : ::syspilot::PdfEngineArgsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::PdfEngineArgs value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::PdfEngineArgs value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end PdfEngineArgs.

// Verbose from PdfEngineFlags version 1.
namespace syspilot {
inline std::array<std::pair<std::string, int>, 0> PdfEngineFlagsPairs = {{
}};
} // namespace syspilot

inline std::string to_string(::syspilot::PdfEngineFlags value) {
    for (auto const& item : ::syspilot::PdfEngineFlagsPairs) {
        if (item.second == static_cast<int>(value)) {
            return item.first;
        }
    }
    return std::to_string(static_cast<int>(value));
}

inline std::ostream& operator<<(std::ostream& os, ::syspilot::PdfEngineFlags value) {
    return os << ::to_string(value);
}

#ifdef QT_CORE_LIB
inline QDebug operator<<(QDebug dbg, ::syspilot::PdfEngineFlags value) {
    auto const text = ::to_string(value);
    dbg << text.c_str();
    return dbg;
}
#endif
// Verbose end PdfEngineFlags.

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

template<>
struct CLIEnumTraits<PdfEngineFlags> {
    static std::string to_string(PdfEngineFlags value)
    {
        return ::to_string(value);
    }

    static std::optional<PdfEngineFlags> from_string(const std::string& value)
    {
        return detail::cli_enum_from_string<PdfEngineFlags>(PdfEngineFlagsPairs, value);
    }
};

using PdfEngineCLIOptions = CLIOptions<PdfEngineCommands, PdfEngineArgs, PdfEngineFlags>;

} // namespace syspilot
