#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QString>

#include <sstream>

#include <process/process.h>
#include <syspilot/verbose/pdfengineargs.h>
#include <syspilot/verbose/processtypes.h>

namespace {

QString current_test_executable()
{
    const QString applicationFilePath = QCoreApplication::applicationFilePath();
    if(!applicationFilePath.isEmpty() && QFileInfo::exists(applicationFilePath)) {
        return applicationFilePath;
    }

    const QString buildRootPath = QDir::current().absoluteFilePath(QStringLiteral("modules/syspilot/syspilot_tests.exe"));
    if(QFileInfo::exists(buildRootPath)) {
        return buildRootPath;
    }

    return QDir::current().absoluteFilePath(QStringLiteral("syspilot_tests.exe"));
}

} // namespace

TEST(ProcessTests, ProcStateStreamsVerboseName)
{
    std::ostringstream stream;

    stream << syspilot::ProcState::WORKING;

    EXPECT_EQ(stream.str(), "WORKING");
}

TEST(ChildProcessTests, PathConstructorStartsIdle)
{
    syspilot::ChildProcess process(QStringLiteral("C:/Projects/fake.exe"));

    EXPECT_EQ(process.state(), syspilot::ProcState::IDLE);
    EXPECT_EQ(process.qt_state(), QProcess::NotRunning);
}

TEST(ChildProcessTests, BinTypeConstructorStartsIdle)
{
    syspilot::ChildProcess process(syspilot::BinType::Elevator);

    EXPECT_EQ(process.state(), syspilot::ProcState::IDLE);
    EXPECT_EQ(process.qt_state(), QProcess::NotRunning);
}

TEST(ChildProcessTests, AddArgumentAllowedOnlyWhileIdle)
{
    syspilot::ChildProcess process(current_test_executable());

    EXPECT_TRUE(process.add_argument(QStringLiteral("--gtest_list_tests")));
    ASSERT_TRUE(process.run()) << process.error_string().toStdString();
    EXPECT_FALSE(process.add_argument(QStringLiteral("--late-argument")));
    EXPECT_TRUE(process.wait());
    EXPECT_EQ(process.state(), syspilot::ProcState::STOPPED);
}

TEST(ChildProcessTests, CLIOptionsCanBeAddedOnlyWhileIdle)
{
    syspilot::PdfEngineCLIOptions options;
    options.set_command(syspilot::PdfEngineCommands::snapshot);
    options.set_argument(syspilot::PdfEngineArgs::output, "result.xml");

    syspilot::ChildProcess process(current_test_executable());
    EXPECT_TRUE(process << options);

    syspilot::ChildProcess runningProcess(current_test_executable());
    ASSERT_TRUE(runningProcess.add_argument(QStringLiteral("--gtest_list_tests")));
    ASSERT_TRUE(runningProcess.run()) << runningProcess.error_string().toStdString();
    EXPECT_FALSE(runningProcess << options);
    EXPECT_TRUE(runningProcess.wait());
}

TEST(ChildProcessTests, InvalidCLIOptionsAreRejected)
{
    char executable[] = "pdfengine";
    char command[] = "missing";
    char* argv[] = {executable, command};
    const syspilot::PdfEngineCLIOptions options(2, argv);

    syspilot::ChildProcess process(current_test_executable());

    EXPECT_FALSE(process << options);
}

TEST(ChildProcessTests, WaitBeforeRunReturnsFalseAndKeepsIdle)
{
    syspilot::ChildProcess process(current_test_executable());

    EXPECT_FALSE(process.wait());
    EXPECT_EQ(process.state(), syspilot::ProcState::IDLE);
}

TEST(ChildProcessTests, StopAndKillBeforeRunKeepIdle)
{
    syspilot::ChildProcess process(current_test_executable());

    process.stop();
    EXPECT_EQ(process.state(), syspilot::ProcState::IDLE);

    process.kill();
    EXPECT_EQ(process.state(), syspilot::ProcState::IDLE);
}

TEST(ChildProcessTests, RunAndWaitOwnsStartedProcess)
{
    syspilot::ChildProcess process(current_test_executable());
    ASSERT_TRUE(process.add_argument(QStringLiteral("--gtest_list_tests")));

    ASSERT_TRUE(process.run()) << process.error_string().toStdString();
    EXPECT_EQ(process.state(), syspilot::ProcState::WORKING);
    EXPECT_GT(process.process_id(), 0);

    EXPECT_TRUE(process.wait());
    EXPECT_EQ(process.state(), syspilot::ProcState::STOPPED);
    EXPECT_EQ(process.exit_status(), QProcess::NormalExit);
    EXPECT_EQ(process.exit_code(), 0);
}

TEST(ProcessTests, InvalidPidStartsStopped)
{
    syspilot::ProcessHandle process(-1);

    EXPECT_EQ(process.process_id(), -1);
    EXPECT_EQ(process.state(), syspilot::ProcState::STOPPED);
    EXPECT_TRUE(process.wait());
}

TEST(ProcessTests, CurrentPidStartsWorking)
{
    syspilot::ProcessHandle process(QCoreApplication::applicationPid());

    EXPECT_EQ(process.process_id(), QCoreApplication::applicationPid());
    EXPECT_EQ(process.state(), syspilot::ProcState::WORKING);
}

TEST(ProcessTests, WaitBlocksUntilExistingPidStops)
{
    syspilot::ChildProcess child(current_test_executable());
    ASSERT_TRUE(child.add_argument(QStringLiteral("--gtest_list_tests")));
    ASSERT_TRUE(child.run()) << child.error_string().toStdString();
    ASSERT_GT(child.process_id(), 0);

    syspilot::ProcessHandle process(child.process_id());
    EXPECT_EQ(process.state(), syspilot::ProcState::WORKING);

    EXPECT_TRUE(process.wait());
    EXPECT_EQ(process.state(), syspilot::ProcState::STOPPED);
    EXPECT_TRUE(child.wait());
}

TEST(ProcessTests, KillOnInvalidPidKeepsStopped)
{
    syspilot::ProcessHandle process(-1);

    process.kill();

    EXPECT_EQ(process.state(), syspilot::ProcState::STOPPED);
}
