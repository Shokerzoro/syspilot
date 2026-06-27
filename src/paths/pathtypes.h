#pragma once

namespace syspilot {

enum class DirType { // Verbosed version 1. Need update(yes/no): no
    Binaries,
    AppRoot,
    Database,
    FileCache,
    Updates,
    LocalMinIO,
};

enum class BinType { // Verbosed version 1. Need update(yes/no): no
    App,
    Elevator,
    PdfEngine,
};

} // namespace syspilot
