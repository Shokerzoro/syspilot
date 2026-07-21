#include "clioptions.h"

#include <syspilot/verbose/elevatorargs.h>
#include <syspilot/verbose/pdfengineargs.h>
#include <syspilot/verbose/uniterargs.h>

namespace syspilot {

template<class Command, class Argument, class Flag>
CLIOptions<Command, Argument, Flag>::CLIOptions() = default;

template<class Command, class Argument, class Flag>
CLIOptions<Command, Argument, Flag>::CLIOptions(int argc, char* argv[])
{
    parse(argc, argv);
}

template<class Command, class Argument, class Flag>
void CLIOptions<Command, Argument, Flag>::set_command(Command command)
{
    command_ = command;
    valid_ = true;
    error_.clear();
}

template<class Command, class Argument, class Flag>
bool CLIOptions<Command, Argument, Flag>::command(Command command) const
{
    return command_.has_value() && *command_ == command;
}

template<class Command, class Argument, class Flag>
bool CLIOptions<Command, Argument, Flag>::has_command() const
{
    return command_.has_value();
}

template<class Command, class Argument, class Flag>
void CLIOptions<Command, Argument, Flag>::set_argument(Argument argument, const std::string& value)
{
    arguments_[argument] = value;
    valid_ = true;
    error_.clear();
}

template<class Command, class Argument, class Flag>
std::string CLIOptions<Command, Argument, Flag>::argument(Argument argument) const
{
    const auto found = arguments_.find(argument);
    if(found == arguments_.end()) {
        return {};
    }

    return found->second;
}

template<class Command, class Argument, class Flag>
bool CLIOptions<Command, Argument, Flag>::has_argument(Argument argument) const
{
    return arguments_.find(argument) != arguments_.end();
}

template<class Command, class Argument, class Flag>
void CLIOptions<Command, Argument, Flag>::set_flag(Flag flag)
{
    flags_.insert(flag);
    valid_ = true;
    error_.clear();
}

template<class Command, class Argument, class Flag>
bool CLIOptions<Command, Argument, Flag>::has_flag(Flag flag) const
{
    return flags_.find(flag) != flags_.end();
}

template<class Command, class Argument, class Flag>
bool CLIOptions<Command, Argument, Flag>::valid() const
{
    return valid_;
}

template<class Command, class Argument, class Flag>
std::string CLIOptions<Command, Argument, Flag>::error() const
{
    return error_;
}

template<class Command, class Argument, class Flag>
QStringList CLIOptions<Command, Argument, Flag>::to_qstring_list() const
{
    QStringList result;

    if(command_.has_value()) {
        result.append(QString::fromStdString(CLIEnumTraits<Command>::to_string(*command_)));
    }

    for(const auto& item : arguments_) {
        result.append(QStringLiteral("--") + QString::fromStdString(CLIEnumTraits<Argument>::to_string(item.first)));
        result.append(QString::fromStdString(item.second));
    }

    for(const auto flag : flags_) {
        result.append(QStringLiteral("-") + QString::fromStdString(CLIEnumTraits<Flag>::to_string(flag)));
    }

    return result;
}

template<class Command, class Argument, class Flag>
void CLIOptions<Command, Argument, Flag>::parse(int argc, char* argv[])
{
    valid_ = true;
    error_.clear();
    command_.reset();
    arguments_.clear();
    flags_.clear();

    for(int index = 1; index < argc; ++index) {
        const std::string token = argv[index] == nullptr ? std::string() : std::string(argv[index]);

        if(token.rfind("--", 0) == 0) {
            const std::string argumentName = token.substr(2);
            const auto argument = CLIEnumTraits<Argument>::from_string(argumentName);
            if(!argument.has_value()) {
                invalidate("Unknown argument: " + argumentName);
                return;
            }

            if(index + 1 >= argc) {
                invalidate("Missing value for argument: " + argumentName);
                return;
            }

            const std::string value = argv[index + 1] == nullptr ? std::string() : std::string(argv[index + 1]);
            if(value.rfind("-", 0) == 0) {
                invalidate("Missing value for argument: " + argumentName);
                return;
            }

            arguments_[*argument] = value;
            ++index;
            continue;
        }

        if(token.rfind("-", 0) == 0) {
            const std::string flagName = token.substr(1);
            const auto flag = CLIEnumTraits<Flag>::from_string(flagName);
            if(!flag.has_value()) {
                invalidate("Unknown flag: " + flagName);
                return;
            }

            flags_.insert(*flag);
            continue;
        }

        if(command_.has_value()) {
            invalidate("Unexpected command token: " + token);
            return;
        }

        const auto command = CLIEnumTraits<Command>::from_string(token);
        if(!command.has_value()) {
            invalidate("Unknown command: " + token);
            return;
        }

        command_ = *command;
    }
}

template<class Command, class Argument, class Flag>
void CLIOptions<Command, Argument, Flag>::invalidate(const std::string& error)
{
    valid_ = false;
    error_ = error;
}

template class CLIOptions<ElevatorCommands, ElevatorArgs, ElevatorFlags>;
template class CLIOptions<PdfEngineCommands, PdfEngineArgs, PdfEngineFlags>;
template class CLIOptions<UniterCommands, UniterArgs, UniterFlags>;

} // namespace syspilot
