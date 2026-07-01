#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QString>

#include <syspilot/verbose/storage.h>

namespace {

void set_storage_test_identity(const QString& application)
{
    QCoreApplication::setOrganizationName(QStringLiteral("UniterSyspilotTests"));
    QCoreApplication::setApplicationName(application);
    QCoreApplication::setApplicationVersion(QString());
}

void clear_storage_test_identity()
{
    QCoreApplication::setOrganizationName(QString());
    QCoreApplication::setApplicationName(QString());
    QCoreApplication::setApplicationVersion(QString());
}

} // namespace

TEST(StorageTests, UpdatesAndReadsVersion)
{
    set_storage_test_identity(QStringLiteral("storage_updates_and_reads_version"));

    syspilot::SettingsStorage storage;
    ASSERT_TRUE(storage.remove_uniter_data());

    ASSERT_TRUE(storage.update_version(QStringLiteral("0.0.2")));

    QString version;
    ASSERT_TRUE(storage.version(version));
    EXPECT_EQ(version, QStringLiteral("0.0.2"));

    ASSERT_TRUE(storage.remove_uniter_data());
    clear_storage_test_identity();
}

TEST(StorageTests, RemoveUniterDataClearsVersion)
{
    set_storage_test_identity(QStringLiteral("storage_remove_uniter_data_clears_version"));

    syspilot::SettingsStorage storage;
    ASSERT_TRUE(storage.update_version(QStringLiteral("1.2.3")));
    ASSERT_TRUE(storage.remove_uniter_data());

    QString version;
    ASSERT_TRUE(storage.version(version));
    EXPECT_EQ(version, QStringLiteral("0.0.0"));

    ASSERT_TRUE(storage.remove_uniter_data());
    clear_storage_test_identity();
}

TEST(StorageTests, MissingIdentityReturnsFalse)
{
    clear_storage_test_identity();

    syspilot::SettingsStorage storage;

    QString version;
    EXPECT_FALSE(storage.version(version));
    EXPECT_FALSE(storage.update_version(QStringLiteral("0.0.2")));
    EXPECT_FALSE(storage.remove_uniter_data());
    EXPECT_FALSE(storage.embed_data());
}

TEST(StorageTests, EmbedDataSetsApplicationVersion)
{
    set_storage_test_identity(QStringLiteral("storage_embed_data_sets_application_version"));

    syspilot::SettingsStorage storage;
    ASSERT_TRUE(storage.remove_uniter_data());
    ASSERT_TRUE(storage.update_version(QStringLiteral("2.0.0")));

    ASSERT_TRUE(storage.embed_data());
    EXPECT_EQ(QCoreApplication::applicationVersion(), QStringLiteral("2.0.0"));

    ASSERT_TRUE(storage.remove_uniter_data());
    clear_storage_test_identity();
}
