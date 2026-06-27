#pragma once

namespace syspilot {

enum class EnvType { // Verbosed version 1. Need update(yes/no): no
    Version,
    AppRoot,
    Binaries,
    Database,
    FileCache,
    Updates,
    LocalMinIO,
    AppExecutable,
    ElevatorExecutable,
    PdfEngineExecutable,
    AppPid,
    ElevatorPid,
    PdfEnginePid,
};

} // namespace syspilot
