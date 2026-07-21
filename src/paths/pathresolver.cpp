#include "pathresolver.h"

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>

namespace syspilot {

PathResolver& PathResolver::instance()
{
    static PathResolver paths;
    return paths;
}

QString PathResolver::resolve(DirType type) const
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
        return temporary_path(QStringLiteral("database"));
    case DirType::FileCache:
        return temporary_path(QStringLiteral("filecache"));
    case DirType::Updates:
        return temporary_path(QStringLiteral("updates"));
    case DirType::LocalMinIO:
        return temporary_path(QStringLiteral("localminio"));
    }

    return QString();
}

bool PathResolver::ensure_exists(DirType type) const
{
    const QString resolvedPath = resolve(type);
    if(resolvedPath.isEmpty()) {
        return false;
    }

    QDir directory;
    return directory.mkpath(resolvedPath);
}

void PathResolver::set(DirType type, const std::filesystem::path& path)
{
    if(path.empty()) {
        overrides_.erase(type);
        return;
    }

    overrides_[type] = path;
}

QString PathResolver::executable(BinType type) const
{
    return resolve(DirType::Binaries) + QStringLiteral("/") + executable_name(type);
}

QString PathResolver::app_root() const
{
    return QDir::cleanPath(resolve(DirType::Binaries) + QStringLiteral("/.."));
}

QString PathResolver::temporary_root() const
{
    const QString temporaryLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    return temporaryLocation.isEmpty() ? QString() : QDir::cleanPath(temporaryLocation);
}

QString PathResolver::temporary_path(const QString& directory) const
{
    const QString root = temporary_root();
    return root.isEmpty() ? QString() : QDir::cleanPath(root + QStringLiteral("/") + directory);
}

QString PathResolver::clean_path(const std::filesystem::path& path) const
{
    return QDir::cleanPath(QString::fromStdString(path.generic_string()));
}

QString PathResolver::executable_name(BinType type) const
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
