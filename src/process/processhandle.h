#pragma once

#include "processtypes.h"

#include <QtGlobal>

namespace syspilot {

class ProcessHandle final {
public:
    // Public API
    explicit ProcessHandle(qint64 pid);

    void kill();
    bool wait();

    ProcState state();
    qint64 process_id() const;

    ProcessHandle(const ProcessHandle&) = delete;
    ProcessHandle(ProcessHandle&&) = delete;
    ProcessHandle& operator=(const ProcessHandle&) = delete;
    ProcessHandle& operator=(ProcessHandle&&) = delete;

private:
    void update_state();

private:
    // Private data
    qint64 pid_;
    ProcState state_;
};

} // namespace syspilot
