#include "childprocess.h"

#include <paths/paths.h>

#include <QDir>

namespace syspilot {

ChildProcess::ChildProcess(BinType type)
    : executablePath_(PathResolver::instance().executable(type))
    , state_(ProcState::IDLE)
{
}

ChildProcess::ChildProcess(const QString& executablePath)
    : executablePath_(QDir::cleanPath(executablePath))
    , state_(ProcState::IDLE)
{
}

bool ChildProcess::add_argument(const QString& argument)
{
    update_state();
    if(state_ != ProcState::IDLE) {
        return false;
    }

    arguments_.append(argument);
    return true;
}

bool ChildProcess::run()
{
    update_state();
    if(state_ != ProcState::IDLE) {
        return false;
    }

    process_.start(executablePath_, arguments_);
    update_state();
    return state_ == ProcState::WORKING;
}

void ChildProcess::stop()
{
    update_state();
    if(process_.state() == QProcess::Starting || process_.state() == QProcess::Running) {
        process_.terminate();
    }
    update_state();
}

void ChildProcess::kill()
{
    update_state();
    if(process_.state() == QProcess::Starting || process_.state() == QProcess::Running) {
        process_.kill();
    }
    update_state();
}

bool ChildProcess::wait()
{
    update_state();
    if(state_ == ProcState::IDLE) {
        return false;
    }

    const bool finished = process_.waitForFinished(-1);
    update_state();
    return finished;
}

ProcState ChildProcess::state()
{
    update_state();
    return state_;
}

QProcess::ProcessState ChildProcess::qt_state() const
{
    return process_.state();
}

qint64 ChildProcess::process_id() const
{
    return process_.processId();
}

int ChildProcess::exit_code() const
{
    return process_.exitCode();
}

QProcess::ExitStatus ChildProcess::exit_status() const
{
    return process_.exitStatus();
}

QProcess::ProcessError ChildProcess::error() const
{
    return process_.error();
}

QString ChildProcess::error_string() const
{
    return process_.errorString();
}

void ChildProcess::update_state()
{
    switch(process_.state()) {
    case QProcess::NotRunning:
        if(state_ == ProcState::WORKING) {
            state_ = ProcState::STOPPED;
        }
        break;
    case QProcess::Starting:
    case QProcess::Running:
        state_ = ProcState::WORKING;
        break;
    }
}

} // namespace syspilot
