#include <gtest/gtest.h>

#include <args/args.h>
#include <environment/environment.h>
#include <logging/logger.h>
#include <paths/paths.h>
#include <process/process.h>
#include <recover/recover.h>
#include <storage/storage.h>

TEST(UmbrellaHeadersTests, ExposeDirectoryPublicApi)
{
    syspilot::CLIOptions<syspilot::ElevatorCommands, syspilot::ElevatorArgs>* options = nullptr;
    syspilot::EnvironmentManager* environment = nullptr;
    syspilot::Logger* logger = nullptr;
    syspilot::PathResolver* paths = nullptr;
    syspilot::ChildProcess* childProcess = nullptr;
    syspilot::ProcessHandle* process = nullptr;
    syspilot::SettingsStorage* storage = nullptr;
    syspilot::RecoveryClass* recovery = nullptr;

    EXPECT_EQ(options, nullptr);
    EXPECT_EQ(environment, nullptr);
    EXPECT_EQ(logger, nullptr);
    EXPECT_EQ(paths, nullptr);
    EXPECT_EQ(childProcess, nullptr);
    EXPECT_EQ(process, nullptr);
    EXPECT_EQ(storage, nullptr);
    EXPECT_EQ(recovery, nullptr);
}
