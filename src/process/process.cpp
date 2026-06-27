#include "process.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

namespace syspilot {

#ifdef Q_OS_WIN
namespace {

struct WindowCloseContext {
    DWORD pid = 0;
    bool sentClose = false;
};

BOOL CALLBACK close_window_for_process(HWND window, LPARAM parameter)
{
    auto* context = reinterpret_cast<WindowCloseContext*>(parameter);
    DWORD windowPid = 0;
    GetWindowThreadProcessId(window, &windowPid);

    if(windowPid == context->pid && IsWindowVisible(window)) {
        PostMessageW(window, WM_CLOSE, 0, 0);
        context->sentClose = true;
    }

    return TRUE;
}

} // namespace
#endif

Process::Process(qint64 pid)
    : pid_(pid)
    , state_(ProcState::STOPPED)
{
    update_state();
}

void Process::kill()
{
    update_state();
    if(state_ != ProcState::WORKING) {
        return;
    }

#ifdef Q_OS_WIN
    WindowCloseContext context;
    context.pid = static_cast<DWORD>(pid_);
    EnumWindows(close_window_for_process, reinterpret_cast<LPARAM>(&context));
#else
    // TODO: Implement and test graceful process shutdown for non-Windows targets.
#endif

    update_state();
}

bool Process::wait()
{
    update_state();
    if(state_ == ProcState::STOPPED) {
        return true;
    }

#ifdef Q_OS_WIN
    HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, static_cast<DWORD>(pid_));
    if(process == nullptr) {
        update_state();
        return state_ == ProcState::STOPPED;
    }

    const DWORD waitResult = WaitForSingleObject(process, INFINITE);
    CloseHandle(process);

    update_state();
    return waitResult == WAIT_OBJECT_0 && state_ == ProcState::STOPPED;
#else
    // TODO: Implement and test blocking wait for non-Windows targets.
    update_state();
    return state_ == ProcState::STOPPED;
#endif
}

ProcState Process::state()
{
    update_state();
    return state_;
}

qint64 Process::process_id() const
{
    return pid_;
}

void Process::update_state()
{
    if(pid_ <= 0) {
        state_ = ProcState::STOPPED;
        return;
    }

#ifdef Q_OS_WIN
    HANDLE process = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, static_cast<DWORD>(pid_));
    if(process == nullptr) {
        state_ = ProcState::STOPPED;
        return;
    }

    DWORD exitCode = 0;
    if(GetExitCodeProcess(process, &exitCode) == 0) {
        CloseHandle(process);
        state_ = ProcState::STOPPED;
        return;
    }

    CloseHandle(process);
    state_ = exitCode == STILL_ACTIVE ? ProcState::WORKING : ProcState::STOPPED;
#else
    // TODO: Implement and test process existence checks for non-Windows targets.
    state_ = ProcState::STOPPED;
#endif
}

} // namespace syspilot
