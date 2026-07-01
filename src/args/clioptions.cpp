#include "clioptions.h"

#include <syspilot/verbose/elevatorargs.h>
#include <syspilot/verbose/pdfengineargs.h>
#include <syspilot/verbose/uniterargs.h>

namespace syspilot {

template<class Command, class Argument>
CLIOptions<Command, Argument>::CLIOptions() = default;

template<class Command, class Argument>
CLIOptions<Command, Argument>::CLIOptions(int argc, char* argv[])
{
    parse(argc, argv);
}

template<class Command, class Argument>
void CLIOptions<Command, Argument>::set_command(Command command)
{
    command_ = command;
    valid_ = true;
    error_.clear();
}

template<class Command, class Argument>
bool CLIOptions<Command, Argument>::command(Command command) const
{
    return command_.has_value() && *command_ == command;
}

template<class Command, class Argument>
bool CLIOptions<Command, Argument>::has_command() const
{
    return command_.has_value();
}

template<class Command, class Argument>
void CLIOptions<Command, Argument>::set_argument(Argument argument, const std::string& value)
{
    arguments_[argument] = value;
    valid_ = true;
    error_.clear();
}

template<class Command, class Argument>
std::string CLIOptions<Command, Argument>::argument(Argument argument) const
{
    const auto found = arguments_.find(argument);
    if(found == arguments_.end()) {
        return {};
    }

    return found->second;
}

template<class Command, class Argument>
bool CLIOptions<Command, Argument>::has_argument(Argument argument) const
{
    return arguments_.find(argument) != arguments_.end();
}

template<class Command, class Argument>
bool CLIOptions<Command, Argument>::valid() const
{
    return valid_;
}

template<class Command, class Argument>
std::string CLIOptions<Command, Argument>::error() const
{
    return error_;
}

template<class Command, class Argument>
QStringList CLIOptions<Command, Argument>::to_qstring_list() const
{
    QStringList result;

    if(command_.has_value()) {
        result.append(QString::fromStdString(CLIEnumTraits<Command>::to_string(*command_)));
    }

    for(const auto& item : arguments_) {
        result.append(QStringLiteral("--") + QString::fromStdString(CLIEnumTraits<Argument>::to_string(item.first)));
        result.append(QString::fromStdString(item.second));
    }

    return result;
}

template<class Command, class Argument>
void CLIOptions<Command, Argument>::parse(int argc, char* argv[])
{
    valid_ = true;
    error_.clear();
    command_.reset();
    arguments_.clear();

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
            if(value.rfind("--", 0) == 0) {
                invalidate("Missing value for argument: " + argumentName);
                return;
            }

            arguments_[*argument] = value;
            ++index;
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

template<class Command, class Argument>
void CLIOptions<Command, Argument>::invalidate(const std::string& error)
{
    valid_ = false;
    error_ = error;
}

template class CLIOptions<ElevatorCommands, ElevatorArgs>;
template class CLIOptions<PdfEngineCommands, PdfEngineArgs>;
template class CLIOptions<UniterCommands, UniterArgs>;

} // namespace syspilot
