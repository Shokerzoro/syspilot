#include "paths.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace syspilot {

Paths& Paths::instance()
{
    static Paths paths;
    return paths;
}

QString Paths::resolve(DirType type) const
{
    const auto override = overrides_.find(type);
    if(override != overrides_.end()) {
        return clean_path(override->second);
    }

    switch(type) {
    case DirType::Binaries:
        return QDir::cleanPath(QCoreApplication::applicationDirPath());
    case DirType::AppRoot:
        return app_root();
    case DirType::Database:
        return app_root() + QStringLiteral("/database");
    case DirType::FileCache:
        return temporary_root() + QStringLiteral("/cache");
    case DirType::Updates:
        return temporary_root() + QStringLiteral("/updates");
    case DirType::LocalMinIO:
        return temporary_root() + QStringLiteral("/local-minio");
    }

    return QString();
}

bool Paths::ensure_exists(DirType type) const
{
    const QString resolvedPath = resolve(type);
    if(resolvedPath.isEmpty()) {
        return false;
    }

    QDir directory;
    return directory.mkpath(resolvedPath);
}

void Paths::set(DirType type, const std::filesystem::path& path)
{
    if(path.empty()) {
        overrides_.erase(type);
        return;
    }

    overrides_[type] = path;
}

QString Paths::executable(BinType type) const
{
    return resolve(DirType::Binaries) + QStringLiteral("/") + executable_name(type);
}

QString Paths::app_root() const
{
    return QDir::cleanPath(resolve(DirType::Binaries) + QStringLiteral("/.."));
}

QString Paths::temporary_root() const
{
    const QString temporaryLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    if(!temporaryLocation.isEmpty()) {
        return QDir::cleanPath(temporaryLocation);
    }

    return QDir::cleanPath(app_root() + QStringLiteral("/tmp"));
}

QString Paths::clean_path(const std::filesystem::path& path) const
{
    return QDir::cleanPath(QString::fromStdString(path.generic_string()));
}

QString Paths::executable_name(BinType type) const
{
    QString name;
    switch(type) {
    case BinType::App:
        name = QStringLiteral("app");
        break;
    case BinType::Elevator:
        name = QStringLiteral("elevator");
        break;
    case BinType::PdfEngine:
        name = QStringLiteral("pdfengine");
        break;
    }

#ifdef Q_OS_WIN
    name += QStringLiteral(".exe");
#endif

    return name;
}

} // namespace syspilot
