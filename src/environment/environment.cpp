#include "environment.h"

#include <paths/paths.h>

#include <QCoreApplication>

namespace syspilot {

Environment& Environment::instance()
{
    static Environment environment;
    return environment;
}

bool Environment::set_env()
{
    if(resolve(EnvType::Version).isEmpty()) {
        return false;
    }

    const EnvType variables[] = {
        EnvType::Version,
        EnvType::AppRoot,
        EnvType::Binaries,
        EnvType::Database,
        EnvType::FileCache,
        EnvType::Updates,
        EnvType::LocalMinIO,
        EnvType::AppExecutable,
        EnvType::ElevatorExecutable,
        EnvType::PdfEngineExecutable,
    };

    for(const EnvType variable : variables) {
        if(!set(variable, resolve(variable))) {
            return false;
        }
    }

    return true;
}

QString Environment::read(EnvType type) const
{
    return qEnvironmentVariable(key(type).constData());
}

bool Environment::has(EnvType type) const
{
    return qEnvironmentVariableIsSet(key(type).constData());
}

bool Environment::set(EnvType type, const QString& value)
{
    const QByteArray variable = key(type);
    const QByteArray data = value.toUtf8();
    return qputenv(variable.constData(), QByteArrayView(data));
}

bool Environment::unset(EnvType type)
{
    return qunsetenv(key(type));
}

QByteArray Environment::key(EnvType type) const
{
    switch(type) {
    case EnvType::Version:
        return QByteArrayLiteral("VERSION");
    case EnvType::AppRoot:
        return QByteArrayLiteral("UNITER_APP_ROOT");
    case EnvType::Binaries:
        return QByteArrayLiteral("UNITER_BINARIES_DIR");
    case EnvType::Database:
        return QByteArrayLiteral("UNITER_DATABASE_DIR");
    case EnvType::FileCache:
        return QByteArrayLiteral("UNITER_FILE_CACHE_DIR");
    case EnvType::Updates:
        return QByteArrayLiteral("UNITER_UPDATES_DIR");
    case EnvType::LocalMinIO:
        return QByteArrayLiteral("UNITER_LOCAL_MINIO_DIR");
    case EnvType::AppExecutable:
        return QByteArrayLiteral("UNITER_APP_EXE");
    case EnvType::ElevatorExecutable:
        return QByteArrayLiteral("UNITER_ELEVATOR_EXE");
    case EnvType::PdfEngineExecutable:
        return QByteArrayLiteral("UNITER_PDFENGINE_EXE");
    case EnvType::AppPid:
        return QByteArrayLiteral("UNITER_APP_PID");
    case EnvType::ElevatorPid:
        return QByteArrayLiteral("UNITER_ELEVATOR_PID");
    case EnvType::PdfEnginePid:
        return QByteArrayLiteral("UNITER_PDFENGINE_PID");
    }

    return QByteArray();
}

QString Environment::resolve(EnvType type) const
{
    auto& paths = Paths::instance();

    switch(type) {
    case EnvType::Version:
        return QCoreApplication::applicationVersion();
    case EnvType::AppRoot:
        return paths.resolve(DirType::AppRoot);
    case EnvType::Binaries:
        return paths.resolve(DirType::Binaries);
    case EnvType::Database:
        return paths.resolve(DirType::Database);
    case EnvType::FileCache:
        return paths.resolve(DirType::FileCache);
    case EnvType::Updates:
        return paths.resolve(DirType::Updates);
    case EnvType::LocalMinIO:
        return paths.resolve(DirType::LocalMinIO);
    case EnvType::AppExecutable:
        return paths.executable(BinType::App);
    case EnvType::ElevatorExecutable:
        return paths.executable(BinType::Elevator);
    case EnvType::PdfEngineExecutable:
        return paths.executable(BinType::PdfEngine);
    case EnvType::AppPid:
    case EnvType::ElevatorPid:
    case EnvType::PdfEnginePid:
        return read(type);
    }

    return QString();
}

} // namespace syspilot
