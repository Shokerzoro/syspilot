#include <gtest/gtest.h>

#include <QStringList>

#include <syspilot/verbose/elevatorargs.h>
#include <syspilot/verbose/pdfengineargs.h>
#include <syspilot/verbose/uniterargs.h>

TEST(CLIOptionsTests, DefaultConstructionAllowsSettersAndGetters)
{
    syspilot::PdfEngineCLIOptions options;

    EXPECT_TRUE(options.valid());
    EXPECT_FALSE(options.has_command());
    EXPECT_FALSE(options.command(syspilot::PdfEngineCommands::snapshot));
    EXPECT_FALSE(options.has_argument(syspilot::PdfEngineArgs::output));
    EXPECT_EQ(options.argument(syspilot::PdfEngineArgs::output), "");

    options.set_command(syspilot::PdfEngineCommands::snapshot);
    options.set_argument(syspilot::PdfEngineArgs::output, "result.xml");

    EXPECT_TRUE(options.command(syspilot::PdfEngineCommands::snapshot));
    EXPECT_TRUE(options.has_argument(syspilot::PdfEngineArgs::output));
    EXPECT_EQ(options.argument(syspilot::PdfEngineArgs::output), "result.xml");
}

TEST(CLIOptionsTests, ParsesCommandAndArgumentValue)
{
    char executable[] = "pdfengine";
    char command[] = "snapshot";
    char argument[] = "--output";
    char value[] = "result.xml";
    char* argv[] = {executable, command, argument, value};

    const syspilot::PdfEngineCLIOptions options(4, argv);

    EXPECT_TRUE(options.valid()) << options.error();
    EXPECT_TRUE(options.command(syspilot::PdfEngineCommands::snapshot));
    EXPECT_EQ(options.argument(syspilot::PdfEngineArgs::output), "result.xml");
}

TEST(CLIOptionsTests, EmitsCommandAndArguments)
{
    syspilot::PdfEngineCLIOptions options;
    options.set_command(syspilot::PdfEngineCommands::snapshot);
    options.set_argument(syspilot::PdfEngineArgs::output, "result.xml");

    const QStringList arguments = options.to_qstring_list();

    ASSERT_EQ(arguments.size(), 3);
    EXPECT_EQ(arguments.at(0), QStringLiteral("snapshot"));
    EXPECT_EQ(arguments.at(1), QStringLiteral("--output"));
    EXPECT_EQ(arguments.at(2), QStringLiteral("result.xml"));
}

TEST(CLIOptionsTests, RejectsUnknownCommand)
{
    char executable[] = "pdfengine";
    char command[] = "unknown";
    char* argv[] = {executable, command};

    const syspilot::PdfEngineCLIOptions options(2, argv);

    EXPECT_FALSE(options.valid());
}

TEST(CLIOptionsTests, RejectsUnknownArgument)
{
    char executable[] = "pdfengine";
    char command[] = "snapshot";
    char argument[] = "--unknown";
    char value[] = "result.xml";
    char* argv[] = {executable, command, argument, value};

    const syspilot::PdfEngineCLIOptions options(4, argv);

    EXPECT_FALSE(options.valid());
}

TEST(CLIOptionsTests, RejectsMissingArgumentValue)
{
    char executable[] = "pdfengine";
    char command[] = "snapshot";
    char argument[] = "--output";
    char* argv[] = {executable, command, argument};

    const syspilot::PdfEngineCLIOptions options(3, argv);

    EXPECT_FALSE(options.valid());
}

TEST(CLIOptionsTests, ParsesElevatorPreparedFlag)
{
    char executable[] = "elevator";
    char command[] = "update";
    char flag[] = "-prepared";
    char* argv[] = {executable, command, flag};

    const syspilot::ElevatorCLIOptions options(3, argv);

    EXPECT_TRUE(options.valid()) << options.error();
    EXPECT_TRUE(options.command(syspilot::ElevatorCommands::update));
    EXPECT_TRUE(options.has_flag(syspilot::ElevatorFlags::prepared));
}

TEST(CLIOptionsTests, SetsAndEmitsFlagOnlyOnce)
{
    syspilot::ElevatorCLIOptions options;
    options.set_command(syspilot::ElevatorCommands::update);
    options.set_argument(syspilot::ElevatorArgs::apppid, "123");
    options.set_flag(syspilot::ElevatorFlags::prepared);
    options.set_flag(syspilot::ElevatorFlags::prepared);

    EXPECT_TRUE(options.has_flag(syspilot::ElevatorFlags::prepared));
    EXPECT_EQ(
        options.to_qstring_list(),
        QStringList({
            QStringLiteral("update"),
            QStringLiteral("--apppid"),
            QStringLiteral("123"),
            QStringLiteral("-prepared")
        }));
}

TEST(CLIOptionsTests, ParsesFlagsCommandsAndArgumentsInAnyOrder)
{
    char executable[] = "elevator";
    char flag[] = "-prepared";
    char command[] = "update";
    char argument[] = "--apppid";
    char value[] = "123";
    char* argv[] = {executable, flag, command, argument, value};

    const syspilot::ElevatorCLIOptions options(5, argv);

    EXPECT_TRUE(options.valid()) << options.error();
    EXPECT_TRUE(options.command(syspilot::ElevatorCommands::update));
    EXPECT_EQ(options.argument(syspilot::ElevatorArgs::apppid), "123");
    EXPECT_TRUE(options.has_flag(syspilot::ElevatorFlags::prepared));
}

TEST(CLIOptionsTests, RejectsUnknownFlag)
{
    char executable[] = "elevator";
    char command[] = "update";
    char flag[] = "-unknown";
    char* argv[] = {executable, command, flag};

    const syspilot::ElevatorCLIOptions options(3, argv);

    EXPECT_FALSE(options.valid());
    EXPECT_EQ(options.error(), "Unknown flag: unknown");
}

TEST(CLIOptionsTests, RejectsFlagWhereArgumentValueIsRequired)
{
    char executable[] = "elevator";
    char command[] = "update";
    char argument[] = "--apppid";
    char flag[] = "-prepared";
    char* argv[] = {executable, command, argument, flag};

    const syspilot::ElevatorCLIOptions options(4, argv);

    EXPECT_FALSE(options.valid());
    EXPECT_EQ(options.error(), "Missing value for argument: apppid");
}

TEST(CLIOptionsTests, ParsesElevatorRestartAndUninstallCommands)
{
    char executable[] = "elevator";
    char restart[] = "restartapp";
    char uninstall[] = "uninstall";
    char* restartArgv[] = {executable, restart};
    char* uninstallArgv[] = {executable, uninstall};

    const syspilot::ElevatorCLIOptions restartOptions(2, restartArgv);
    const syspilot::ElevatorCLIOptions uninstallOptions(2, uninstallArgv);

    EXPECT_TRUE(restartOptions.valid()) << restartOptions.error();
    EXPECT_TRUE(restartOptions.command(syspilot::ElevatorCommands::restartapp));
    EXPECT_TRUE(uninstallOptions.valid()) << uninstallOptions.error();
    EXPECT_TRUE(uninstallOptions.command(syspilot::ElevatorCommands::uninstall));
}

TEST(CLIOptionsTests, EmitsUniterStatusFlags)
{
    syspilot::UniterCLIOptions options;
    options.set_command(syspilot::UniterCommands::run);
    options.set_flag(syspilot::UniterFlags::updated);

    EXPECT_EQ(
        options.to_qstring_list(),
        QStringList({QStringLiteral("run"), QStringLiteral("-updated")}));
}

TEST(CLIOptionsTests, ParsesElevatorAppPidArgument)
{
    char executable[] = "elevator";
    char command[] = "update";
    char argument[] = "--apppid";
    char value[] = "123";
    char* argv[] = {executable, command, argument, value};

    const syspilot::ElevatorCLIOptions options(4, argv);

    EXPECT_TRUE(options.valid()) << options.error();
    EXPECT_TRUE(options.command(syspilot::ElevatorCommands::update));
    EXPECT_TRUE(options.has_argument(syspilot::ElevatorArgs::apppid));
    EXPECT_EQ(options.argument(syspilot::ElevatorArgs::apppid), "123");
}
