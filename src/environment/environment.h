#pragma once

#include "environmenttypes.h"

#include <QByteArray>
#include <QString>

namespace syspilot {

class Environment final {
public:
    // Public API
    static Environment& instance();

    bool set_env();

    QString read(EnvType type) const;
    bool has(EnvType type) const;
    bool set(EnvType type, const QString& value);
    bool unset(EnvType type);

    Environment(const Environment&) = delete;
    Environment(Environment&&) = delete;
    Environment& operator=(const Environment&) = delete;
    Environment& operator=(Environment&&) = delete;

private:
    Environment() = default;

    QByteArray key(EnvType type) const;
    QString resolve(EnvType type) const;
};

} // namespace syspilot
