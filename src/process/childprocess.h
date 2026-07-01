#pragma once

#include <args/args.h>
#include "processtypes.h"

#include <paths/paths.h>

#include <QProcess>
#include <QString>
#include <QStringList>

namespace syspilot {

class ChildProcess final {
public:
    // Public API
    explicit ChildProcess(BinType type);
    explicit ChildProcess(const QString& executablePath);

    bool add_argument(const QString& argument);
    template<class Command, class Argument>
    bool operator<<(const CLIOptions<Command, Argument>& options);

    bool run();
    bool wait_started(int timeoutMs = 3000);
    void stop();
    void kill();
    bool wait();

    ProcState state();
    QProcess::ProcessState qt_state() const;
    qint64 process_id() const;
    int exit_code() const;
    QProcess::ExitStatus exit_status() const;
    QProcess::ProcessError error() const;
    QString error_string() const;

    ChildProcess(const ChildProcess&) = delete;
    ChildProcess(ChildProcess&&) = delete;
    ChildProcess& operator=(const ChildProcess&) = delete;
    ChildProcess& operator=(ChildProcess&&) = delete;

private:
    void update_state();

private:
    // Private data
    QString executablePath_;
    QStringList arguments_;
    QProcess process_;
    ProcState state_;
};

template<class Command, class Argument>
bool ChildProcess::operator<<(const CLIOptions<Command, Argument>& options)
{
    update_state();
    if(state_ != ProcState::IDLE || !options.valid()) {
        return false;
    }

    arguments_.append(options.to_qstring_list());
    return true;
}

} // namespace syspilot
