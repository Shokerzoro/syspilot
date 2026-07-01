#include <gtest/gtest.h>

#include <recover/recover.h>

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QIODevice>

#include <filesystem>

namespace {

class ScopedTempTree {
public:
    explicit ScopedTempTree(const QString& name)
        : root_(QDir::temp().absoluteFilePath(QStringLiteral("syspilot-recovery-tests/") + name))
    {
        QDir(root_).removeRecursively();
        QDir().mkpath(root_);
    }

    ~ScopedTempTree()
    {
        QDir(root_).removeRecursively();
    }

    std::filesystem::path path(const QString& child = QString()) const
    {
        if(child.isEmpty()) {
            return std::filesystem::path(root_.toStdString());
        }

        return std::filesystem::path(QDir(root_).absoluteFilePath(child).toStdString());
    }

private:
    QString root_;
};

void write_file(const std::filesystem::path& path, const QByteArray& data)
{
    QDir().mkpath(QString::fromStdString(path.parent_path().generic_string()));

    QFile file(QString::fromStdString(path.generic_string()));
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    ASSERT_EQ(file.write(data), data.size());
}

QByteArray read_file(const std::filesystem::path& path)
{
    QFile file(QString::fromStdString(path.generic_string()));
    EXPECT_TRUE(file.open(QIODevice::ReadOnly));
    return file.readAll();
}

TEST(RecoveryClassTests, PrepareCreatesBackupCopy)
{
    ScopedTempTree tree(QStringLiteral("prepare-copy"));
    const auto appDir = tree.path(QStringLiteral("app"));
    const auto tempDir = tree.path(QStringLiteral("temp"));
    std::filesystem::create_directories(appDir);
    std::filesystem::create_directories(tempDir);
    write_file(appDir / "bin" / "app.exe", "app");

    syspilot::RecoveryClass recovery;
    ASSERT_NO_THROW(recovery.prepare(appDir, tempDir));

    EXPECT_TRUE(std::filesystem::is_regular_file(tempDir / "recover" / "bin" / "app.exe"));
    EXPECT_EQ(read_file(tempDir / "recover" / "bin" / "app.exe"), QByteArray("app"));
}

TEST(RecoveryClassTests, PrepareClearsExistingRecoveryDirectory)
{
    ScopedTempTree tree(QStringLiteral("prepare-clears"));
    const auto appDir = tree.path(QStringLiteral("app"));
    const auto tempDir = tree.path(QStringLiteral("temp"));
    std::filesystem::create_directories(appDir);
    std::filesystem::create_directories(tempDir / "recover");
    write_file(tempDir / "recover" / "stale.txt", "stale");
    write_file(appDir / "fresh.txt", "fresh");

    syspilot::RecoveryClass recovery;
    ASSERT_NO_THROW(recovery.prepare(appDir, tempDir));

    EXPECT_FALSE(std::filesystem::exists(tempDir / "recover" / "stale.txt"));
    EXPECT_EQ(read_file(tempDir / "recover" / "fresh.txt"), QByteArray("fresh"));
}

TEST(RecoveryClassTests, RelaxDeletesRecoveryDirectory)
{
    ScopedTempTree tree(QStringLiteral("relax"));
    const auto appDir = tree.path(QStringLiteral("app"));
    const auto tempDir = tree.path(QStringLiteral("temp"));
    std::filesystem::create_directories(appDir);
    std::filesystem::create_directories(tempDir);
    write_file(appDir / "file.txt", "data");

    syspilot::RecoveryClass recovery;
    ASSERT_NO_THROW(recovery.prepare(appDir, tempDir));
    ASSERT_NO_THROW(recovery.relax());

    EXPECT_FALSE(std::filesystem::exists(tempDir / "recover"));
}

TEST(RecoveryClassTests, RecoverRestoresAppDirectoryAndDeletesRecoveryDirectory)
{
    ScopedTempTree tree(QStringLiteral("recover"));
    const auto appDir = tree.path(QStringLiteral("app"));
    const auto tempDir = tree.path(QStringLiteral("temp"));
    std::filesystem::create_directories(appDir);
    std::filesystem::create_directories(tempDir);
    write_file(appDir / "bin" / "app.exe", "original");

    syspilot::RecoveryClass recovery;
    ASSERT_NO_THROW(recovery.prepare(appDir, tempDir));

    std::filesystem::remove_all(appDir);
    std::filesystem::create_directories(appDir);
    write_file(appDir / "bin" / "app.exe", "changed");
    write_file(appDir / "new.txt", "new");

    ASSERT_NO_THROW(recovery.recover());

    EXPECT_EQ(read_file(appDir / "bin" / "app.exe"), QByteArray("original"));
    EXPECT_FALSE(std::filesystem::exists(appDir / "new.txt"));
    EXPECT_FALSE(std::filesystem::exists(tempDir / "recover"));
}

TEST(RecoveryClassTests, SetdataUsesExistingRecoveryDirectory)
{
    ScopedTempTree tree(QStringLiteral("setdata"));
    const auto appDir = tree.path(QStringLiteral("app"));
    const auto tempDir = tree.path(QStringLiteral("temp"));
    std::filesystem::create_directories(appDir);
    std::filesystem::create_directories(tempDir / "recover");
    write_file(tempDir / "recover" / "bin" / "app.exe", "backup");
    write_file(appDir / "bin" / "app.exe", "changed");

    syspilot::RecoveryClass recovery;
    ASSERT_NO_THROW(recovery.setdata(appDir, tempDir));
    ASSERT_NO_THROW(recovery.recover());

    EXPECT_EQ(read_file(appDir / "bin" / "app.exe"), QByteArray("backup"));
    EXPECT_FALSE(std::filesystem::exists(tempDir / "recover"));
}

} // namespace