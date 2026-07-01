#include "settingsstorage.h"

#include <QCoreApplication>

namespace syspilot {

SettingsStorage::SettingsStorage()
{
#ifdef UNITER_ORGANIZATION_NAME
    organization_ = QString::fromUtf8(UNITER_ORGANIZATION_NAME);
#endif

#ifdef UNITER_APPLICATION_NAME
    application_ = QString::fromUtf8(UNITER_APPLICATION_NAME);
#endif

    if(organization_.isEmpty()) {
        organization_ = QCoreApplication::organizationName();
    }

    if(application_.isEmpty()) {
        application_ = QCoreApplication::applicationName();
    }
}

bool SettingsStorage::embed_data()
{
    if(!has_identity()) {
        return false;
    }

    QCoreApplication::setOrganizationName(organization_);
    QCoreApplication::setApplicationName(application_);

    QSettings settings = make_settings();
    QString currentVersion = settings.value("Version").toString();

    if(currentVersion.isEmpty()) {
        currentVersion = QStringLiteral("0.0.0");
        settings.setValue("Version", currentVersion);
        settings.sync();
    }

    QCoreApplication::setApplicationVersion(currentVersion);
    return true;
}

bool SettingsStorage::version(QString& version) const
{
    if(!has_identity()) {
        return false;
    }

    QSettings settings = make_settings();
    version = settings.value("Version", QStringLiteral("0.0.0")).toString();
    return true;
}

bool SettingsStorage::update_version(const QString& version)
{
    if(!has_identity()) {
        return false;
    }

    QSettings settings = make_settings();
    settings.setValue("Version", version);
    settings.sync();
    return true;
}

bool SettingsStorage::remove_uniter_data()
{
    if(!has_identity()) {
        return false;
    }

    QSettings settings = make_settings();
    settings.clear();
    settings.sync();
    return true;
}

bool SettingsStorage::has_identity() const
{
    return !organization_.isEmpty() && !application_.isEmpty();
}

QSettings SettingsStorage::make_settings() const
{
    return QSettings(
        QSettings::NativeFormat,
        QSettings::UserScope,
        organization_,
        application_
    );
}

} // namespace syspilot
