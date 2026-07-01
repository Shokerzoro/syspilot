#pragma once

#include "environmenttypes.h"

#include <QByteArray>
#include <QString>

namespace syspilot {

class EnvironmentManager final {
public:
    // Public API
    static EnvironmentManager& instance();

    bool set_env();

    QString read(EnvType type) const;
    bool has(EnvType type) const;
    bool set(EnvType type, const QString& value);
    bool unset(EnvType type);

    EnvironmentManager(const EnvironmentManager&) = delete;
    EnvironmentManager(EnvironmentManager&&) = delete;
    EnvironmentManager& operator=(const EnvironmentManager&) = delete;
    EnvironmentManager& operator=(EnvironmentManager&&) = delete;

private:
    EnvironmentManager() = default;

    QByteArray key(EnvType type) const;
    QString resolve(EnvType type) const;
};

} // namespace syspilot
