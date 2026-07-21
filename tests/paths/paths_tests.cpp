#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QDir>
#include <QStandardPaths>
#include <QString>

#include <filesystem>
#include <sstream>

#include <syspilot/verbose/pathtypes.h>
#include <paths/paths.h>

TEST(PathsTests, InstanceReturnsSingleObject)
{
    auto& first = syspilot::PathResolver::instance();
    auto& second = syspilot::PathResolver::instance();

    EXPECT_EQ(&first, &second);
}

TEST(PathsTests, ResolvesBinariesDirectoryToApplicationDirectory)
{
    const QString expected = QDir::cleanPath(QCoreApplication::applicationDirPath());

    EXPECT_EQ(
        syspilot::PathResolver::instance().resolve(syspilot::DirType::Binaries),
        expected
    );
}

TEST(PathsTests, ResolvesAppRootAsBinariesParentDirectory)
{
    const QString expected = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + QStringLiteral("/..")
    );

    EXPECT_EQ(
        syspilot::PathResolver::instance().resolve(syspilot::DirType::AppRoot),
        expected
    );
}

TEST(PathsTests, ResolvesRuntimeDataDirectoriesUnderTemporaryDirectory)
{
    const QString temporaryLocation = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    auto& paths = syspilot::PathResolver::instance();

    if(temporaryLocation.isEmpty()) {
        EXPECT_TRUE(paths.resolve(syspilot::DirType::Database).isEmpty());
        EXPECT_TRUE(paths.resolve(syspilot::DirType::FileCache).isEmpty());
        EXPECT_TRUE(paths.resolve(syspilot::DirType::Updates).isEmpty());
        EXPECT_TRUE(paths.resolve(syspilot::DirType::LocalMinIO).isEmpty());
        return;
    }

    const QString temporaryRoot = QDir::cleanPath(temporaryLocation);
    EXPECT_EQ(paths.resolve(syspilot::DirType::Database), temporaryRoot + QStringLiteral("/database"));
    EXPECT_EQ(paths.resolve(syspilot::DirType::FileCache), temporaryRoot + QStringLiteral("/filecache"));
    EXPECT_EQ(paths.resolve(syspilot::DirType::Updates), temporaryRoot + QStringLiteral("/updates"));
    EXPECT_EQ(paths.resolve(syspilot::DirType::LocalMinIO), temporaryRoot + QStringLiteral("/localminio"));
}

TEST(PathsTests, RuntimeDataDirectoriesDoNotResolveUnderAppRoot)
{
    const QString appRoot = syspilot::PathResolver::instance().resolve(syspilot::DirType::AppRoot);
    auto& paths = syspilot::PathResolver::instance();

    EXPECT_FALSE(paths.resolve(syspilot::DirType::Database).startsWith(appRoot + QStringLiteral("/")));
    EXPECT_FALSE(paths.resolve(syspilot::DirType::FileCache).startsWith(appRoot + QStringLiteral("/")));
    EXPECT_FALSE(paths.resolve(syspilot::DirType::LocalMinIO).startsWith(appRoot + QStringLiteral("/")));
}

TEST(PathsTests, SetterOverridesDirectoryResolving)
{
    auto& paths = syspilot::PathResolver::instance();
    const QString expected = QDir::cleanPath(QStringLiteral("C:/Projects/custom-cache"));

    paths.set(syspilot::DirType::FileCache, std::filesystem::path("C:/Projects/custom-cache"));

    EXPECT_EQ(paths.resolve(syspilot::DirType::FileCache), expected);

    paths.set(syspilot::DirType::FileCache, std::filesystem::path());
}

TEST(PathsTests, EnsureExistsCreatesResolvedDirectory)
{
    auto& paths = syspilot::PathResolver::instance();
    const QString updates = paths.resolve(syspilot::DirType::Updates);
    QDir(updates).removeRecursively();

    ASSERT_TRUE(paths.ensure_exists(syspilot::DirType::Updates));
    EXPECT_TRUE(QDir(updates).exists());

    QDir(updates).removeRecursively();
}

TEST(PathsTests, ResolvesApplicationExecutableUnderBinariesDirectory)
{
    const QString binaries = syspilot::PathResolver::instance().resolve(syspilot::DirType::Binaries);

    EXPECT_EQ(
        syspilot::PathResolver::instance().executable(syspilot::BinType::App),
        binaries + QStringLiteral("/app.exe")
    );
}

TEST(PathsTests, ResolvesElevatorExecutableUnderBinariesDirectory)
{
    const QString binaries = syspilot::PathResolver::instance().resolve(syspilot::DirType::Binaries);

    EXPECT_EQ(
        syspilot::PathResolver::instance().executable(syspilot::BinType::Elevator),
        binaries + QStringLiteral("/elevator.exe")
    );
}

TEST(PathsTests, ResolvesPdfEngineExecutableUnderBinariesDirectory)
{
    const QString binaries = syspilot::PathResolver::instance().resolve(syspilot::DirType::Binaries);

    EXPECT_EQ(
        syspilot::PathResolver::instance().executable(syspilot::BinType::PdfEngine),
        binaries + QStringLiteral("/pdfengine.exe")
    );
}

TEST(PathsTests, DirTypeStreamsVerboseName)
{
    std::ostringstream stream;

    stream << syspilot::DirType::FileCache;

    EXPECT_EQ(stream.str(), "FileCache");
}

TEST(PathsTests, BinTypeStreamsVerboseName)
{
    std::ostringstream stream;

    stream << syspilot::BinType::PdfEngine;

    EXPECT_EQ(stream.str(), "PdfEngine");
}
