#pragma once

#include "pathtypes.h"

#include <QString>

#include <filesystem>
#include <map>

namespace syspilot {

class PathResolver final {
public:
    // Public API
    static PathResolver& instance();

    QString resolve(DirType type) const;
    bool ensure_exists(DirType type) const;
    void set(DirType type, const std::filesystem::path& path);

    QString executable(BinType type) const;

    PathResolver(const PathResolver&) = delete;
    PathResolver(PathResolver&&) = delete;
    PathResolver& operator=(const PathResolver&) = delete;
    PathResolver& operator=(PathResolver&&) = delete;

private:
    PathResolver() = default;

    QString app_root() const;
    QString temporary_root() const;
    QString temporary_path(const QString& directory) const;
    QString clean_path(const std::filesystem::path& path) const;
    QString executable_name(BinType type) const;

private:
    // Private data
    std::map<DirType, std::filesystem::path> overrides_;
};

} // namespace syspilot
