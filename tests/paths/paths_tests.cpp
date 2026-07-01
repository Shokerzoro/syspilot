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

TEST(PathsTests, ResolvesDatabaseDirectoryUnderAppRoot)
{
    const QString appRoot = QDir::cleanPath(
        QCoreApplication::applicationDirPath() + QStringLiteral("/..")
    );

    EXPECT_EQ(
        syspilot::PathResolver::instance().resolve(syspilot::DirType::Database),
        appRoot + QStringLiteral("/database")
    );
}

TEST(PathsTests, ResolvesTemporaryWorkDirectoriesUnderTemporaryDirectory)
{
    const QString temporaryRoot = QDir::cleanPath(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
    auto& paths = syspilot::PathResolver::instance();

    EXPECT_EQ(paths.resolve(syspilot::DirType::FileCache), temporaryRoot + QStringLiteral("/cache"));
    EXPECT_EQ(paths.resolve(syspilot::DirType::Updates), temporaryRoot + QStringLiteral("/updates"));
    EXPECT_EQ(paths.resolve(syspilot::DirType::LocalMinIO), temporaryRoot + QStringLiteral("/local-minio"));
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
