#pragma once

#include "pathtypes.h"

#include <QString>

#include <filesystem>
#include <map>

namespace syspilot {

class Paths final {
public:
    // Public API
    static Paths& instance();

    QString resolve(DirType type) const;
    bool ensure_exists(DirType type) const;
    void set(DirType type, const std::filesystem::path& path);

    QString executable(BinType type) const;

    Paths(const Paths&) = delete;
    Paths(Paths&&) = delete;
    Paths& operator=(const Paths&) = delete;
    Paths& operator=(Paths&&) = delete;

private:
    Paths() = default;

    QString app_root() const;
    QString temporary_root() const;
    QString clean_path(const std::filesystem::path& path) const;
    QString executable_name(BinType type) const;

private:
    // Private data
    std::map<DirType, std::filesystem::path> overrides_;
};

} // namespace syspilot
