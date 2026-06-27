#pragma once

#include "processtypes.h"

#include <QtGlobal>

namespace syspilot {

class Process final {
public:
    // Public API
    explicit Process(qint64 pid);

    void kill();
    bool wait();

    ProcState state();
    qint64 process_id() const;

    Process(const Process&) = delete;
    Process(Process&&) = delete;
    Process& operator=(const Process&) = delete;
    Process& operator=(Process&&) = delete;

private:
    void update_state();

private:
    // Private data
    qint64 pid_;
    ProcState state_;
};

} // namespace syspilot
