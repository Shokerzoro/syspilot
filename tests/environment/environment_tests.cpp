#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QString>

#include <sstream>

#include <paths/paths.h>
#include <syspilot/verbose/environmenttypes.h>
#include <environment/environment.h>

namespace {

void unset_standard_environment()
{
    auto& environment = syspilot::Environment::instance();
    environment.unset(syspilot::EnvType::Version);
    environment.unset(syspilot::EnvType::AppRoot);
    environment.unset(syspilot::EnvType::Binaries);
    environment.unset(syspilot::EnvType::Database);
    environment.unset(syspilot::EnvType::FileCache);
    environment.unset(syspilot::EnvType::Updates);
    environment.unset(syspilot::EnvType::LocalMinIO);
    environment.unset(syspilot::EnvType::AppExecutable);
    environment.unset(syspilot::EnvType::ElevatorExecutable);
    environment.unset(syspilot::EnvType::PdfEngineExecutable);
    environment.unset(syspilot::EnvType::AppPid);
    environment.unset(syspilot::EnvType::ElevatorPid);
    environment.unset(syspilot::EnvType::PdfEnginePid);
    environment.unset(syspilot::EnvType::ServerType);
    environment.unset(syspilot::EnvType::ServerIp);
}

} // namespace

TEST(EnvironmentTests, EnvTypeStreamsVerboseName)
{
    std::ostringstream stream;

    stream << syspilot::EnvType::Database;

    EXPECT_EQ(stream.str(), "Database");
}

TEST(EnvironmentTests, SetReadHasAndUnsetVersion)
{
    auto& environment = syspilot::Environment::instance();
    environment.unset(syspilot::EnvType::Version);

    ASSERT_TRUE(environment.set(syspilot::EnvType::Version, QStringLiteral("1.2.3")));
    EXPECT_TRUE(environment.has(syspilot::EnvType::Version));
    EXPECT_EQ(environment.read(syspilot::EnvType::Version), QStringLiteral("1.2.3"));

    ASSERT_TRUE(environment.unset(syspilot::EnvType::Version));
    EXPECT_FALSE(environment.has(syspilot::EnvType::Version));
}

TEST(EnvironmentTests, SetEnvFillsVersionFromApplicationVersion)
{
    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QStringLiteral("2.0.0"));

    auto& environment = syspilot::Environment::instance();

    ASSERT_TRUE(environment.set_env());
    EXPECT_EQ(environment.read(syspilot::EnvType::Version), QStringLiteral("2.0.0"));

    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QString());
}

TEST(EnvironmentTests, SetEnvFillsDirectoriesFromPaths)
{
    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QStringLiteral("2.0.0"));

    auto& environment = syspilot::Environment::instance();
    auto& paths = syspilot::Paths::instance();

    ASSERT_TRUE(environment.set_env());
    EXPECT_EQ(environment.read(syspilot::EnvType::AppRoot), paths.resolve(syspilot::DirType::AppRoot));
    EXPECT_EQ(environment.read(syspilot::EnvType::Binaries), paths.resolve(syspilot::DirType::Binaries));
    EXPECT_EQ(environment.read(syspilot::EnvType::Database), paths.resolve(syspilot::DirType::Database));
    EXPECT_EQ(environment.read(syspilot::EnvType::FileCache), paths.resolve(syspilot::DirType::FileCache));
    EXPECT_EQ(environment.read(syspilot::EnvType::Updates), paths.resolve(syspilot::DirType::Updates));
    EXPECT_EQ(environment.read(syspilot::EnvType::LocalMinIO), paths.resolve(syspilot::DirType::LocalMinIO));

    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QString());
}

TEST(EnvironmentTests, SetEnvFillsExecutablesFromPaths)
{
    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QStringLiteral("2.0.0"));

    auto& environment = syspilot::Environment::instance();
    auto& paths = syspilot::Paths::instance();

    ASSERT_TRUE(environment.set_env());
    EXPECT_EQ(environment.read(syspilot::EnvType::AppExecutable), paths.executable(syspilot::BinType::App));
    EXPECT_EQ(environment.read(syspilot::EnvType::ElevatorExecutable), paths.executable(syspilot::BinType::Elevator));
    EXPECT_EQ(environment.read(syspilot::EnvType::PdfEngineExecutable), paths.executable(syspilot::BinType::PdfEngine));

    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QString());
}

TEST(EnvironmentTests, SetEnvReturnsFalseWhenApplicationVersionIsEmpty)
{
    unset_standard_environment();
    QCoreApplication::setApplicationVersion(QString());

    EXPECT_FALSE(syspilot::Environment::instance().set_env());

    unset_standard_environment();
}

TEST(EnvironmentTests, CanSetReadAndUnsetExecutablePidVariables)
{
    auto& environment = syspilot::Environment::instance();
    unset_standard_environment();

    ASSERT_TRUE(environment.set(syspilot::EnvType::AppPid, QStringLiteral("101")));
    ASSERT_TRUE(environment.set(syspilot::EnvType::ElevatorPid, QStringLiteral("202")));
    ASSERT_TRUE(environment.set(syspilot::EnvType::PdfEnginePid, QStringLiteral("303")));

    EXPECT_EQ(environment.read(syspilot::EnvType::AppPid), QStringLiteral("101"));
    EXPECT_EQ(environment.read(syspilot::EnvType::ElevatorPid), QStringLiteral("202"));
    EXPECT_EQ(environment.read(syspilot::EnvType::PdfEnginePid), QStringLiteral("303"));

    EXPECT_TRUE(environment.has(syspilot::EnvType::AppPid));
    EXPECT_TRUE(environment.has(syspilot::EnvType::ElevatorPid));
    EXPECT_TRUE(environment.has(syspilot::EnvType::PdfEnginePid));

    unset_standard_environment();
}

TEST(EnvironmentTests, CanSetReadAndUnsetServerVariables)
{
    auto& environment = syspilot::Environment::instance();
    unset_standard_environment();

    ASSERT_TRUE(environment.set(syspilot::EnvType::ServerType, QStringLiteral("remote")));
    ASSERT_TRUE(environment.set(syspilot::EnvType::ServerIp, QStringLiteral("192.168.1.10")));

    EXPECT_EQ(environment.read(syspilot::EnvType::ServerType), QStringLiteral("remote"));
    EXPECT_EQ(environment.read(syspilot::EnvType::ServerIp), QStringLiteral("192.168.1.10"));

    EXPECT_TRUE(environment.has(syspilot::EnvType::ServerType));
    EXPECT_TRUE(environment.has(syspilot::EnvType::ServerIp));

    unset_standard_environment();
}
