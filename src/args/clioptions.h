#pragma once

#include <QString>
#include <QStringList>

#include <array>
#include <map>
#include <optional>
#include <sstream>
#include <string>
#include <utility>

namespace syspilot {

template<class Enum>
struct CLIEnumTraits;

namespace detail {

template<class Enum, std::size_t Size>
std::optional<Enum> cli_enum_from_string(const std::array<std::pair<std::string, int>, Size>& pairs,
                                         const std::string& value)
{
    for(const auto& item : pairs) {
        if(item.first == value) {
            return static_cast<Enum>(item.second);
        }
    }

    return std::nullopt;
}

} // namespace detail

template<class Command, class Argument>
class CLIOptions {
public:
    // Constructors
    CLIOptions();
    CLIOptions(int argc, char* argv[]);

    // Commands
    void set_command(Command command);
    bool command(Command command) const;
    bool has_command() const;

    // Arguments
    void set_argument(Argument argument, const std::string& value);
    std::string argument(Argument argument) const;
    bool has_argument(Argument argument) const;

    // State
    bool valid() const;
    std::string error() const;

    // Conversion
    QStringList to_qstring_list() const;

private:
    void parse(int argc, char* argv[]);
    void invalidate(const std::string& error);

private:
    std::optional<Command> command_;
    std::map<Argument, std::string> arguments_;
    bool valid_ = true;
    std::string error_;
};

} // namespace syspilot
