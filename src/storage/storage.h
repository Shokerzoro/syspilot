#pragma once

#include <QSettings>
#include <QString>

namespace syspilot {

class Storage final {
public:
    // Public API
    Storage();

    bool embed_data();
    bool version(QString& version) const;
    bool update_version(const QString& version);
    bool remove_uniter_data();

private:
    bool has_identity() const;
    QSettings make_settings() const;

private:
    // Private data
    QString organization_;
    QString application_;
};

} // namespace syspilot
