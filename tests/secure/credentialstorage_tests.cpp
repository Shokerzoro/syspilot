#include <gtest/gtest.h>

#include <QByteArray>
#include <QString>
#include <QUuid>

#include <utility>

#include <secure/secure.h>

namespace {

QString unique_target(const QString& name)
{
    return QStringLiteral("UnitedSystems.Uniter.Tests/")
        + name
        + QStringLiteral("/")
        + QUuid::createUuid().toString(QUuid::WithoutBraces);
}

class CredentialCleanup final {
public:
    explicit CredentialCleanup(QString target)
        : target_(std::move(target))
    {
    }

    ~CredentialCleanup()
    {
        (void)storage_.remove(target_);
    }

    CredentialCleanup(const CredentialCleanup&) = delete;
    CredentialCleanup(CredentialCleanup&&) = delete;
    CredentialCleanup& operator=(const CredentialCleanup&) = delete;
    CredentialCleanup& operator=(CredentialCleanup&&) = delete;

private:
    syspilot::CredentialStorage storage_;
    QString target_;
};

} // namespace

#ifdef Q_OS_WIN

TEST(CredentialStorageTests, WritesReadsReplacesAndRemovesCredential)
{
    syspilot::CredentialStorage storage;
    const QString target = unique_target(QStringLiteral("round_trip"));
    CredentialCleanup cleanup(target);

    const auto missing = storage.read(target);
    EXPECT_EQ(missing.error.status, syspilot::CredentialStatus::NotFound);
    EXPECT_FALSE(missing.credential.has_value());
    EXPECT_NE(missing.error.native_code, 0U);

    const QByteArray binarySecret("first\0secret", 12);
    ASSERT_TRUE(storage.write(
        target,
        {QString::fromUtf8("user-\xD1\x82\xD0\xB5\xD1\x81\xD1\x82"), binarySecret}).ok());

    const auto firstRead = storage.read(target);
    ASSERT_TRUE(firstRead.ok());
    EXPECT_EQ(firstRead.credential->username, QString::fromUtf8("user-\xD1\x82\xD0\xB5\xD1\x81\xD1\x82"));
    EXPECT_EQ(firstRead.credential->secret, binarySecret);

    ASSERT_TRUE(storage.write(target, {QString(), QByteArray()}).ok());
    const auto replacement = storage.read(target);
    ASSERT_TRUE(replacement.ok());
    EXPECT_TRUE(replacement.credential->username.isEmpty());
    EXPECT_TRUE(replacement.credential->secret.isEmpty());

    EXPECT_TRUE(storage.remove(target).ok());
    EXPECT_EQ(storage.read(target).error.status, syspilot::CredentialStatus::NotFound);
    EXPECT_EQ(storage.remove(target).status, syspilot::CredentialStatus::NotFound);
}

TEST(CredentialStorageTests, RejectsInvalidArgumentsAndOversizedSecret)
{
    syspilot::CredentialStorage storage;
    const QString target = unique_target(QStringLiteral("validation"));

    EXPECT_EQ(
        storage.write(QString(), {}).status,
        syspilot::CredentialStatus::InvalidArgument);

    QString targetWithNull = QStringLiteral("target");
    targetWithNull.append(QChar(u'\0'));
    targetWithNull.append(QStringLiteral("suffix"));
    EXPECT_EQ(
        storage.read(targetWithNull).error.status,
        syspilot::CredentialStatus::InvalidArgument);

    EXPECT_EQ(
        storage.read(QString(32768, QChar(u'a'))).error.status,
        syspilot::CredentialStatus::InvalidArgument);
    EXPECT_EQ(
        storage.write(target, {QString(514, QChar(u'u')), QByteArray()}).status,
        syspilot::CredentialStatus::InvalidArgument);
    EXPECT_EQ(
        storage.write(target, {QString(), QByteArray(2561, 's')}).status,
        syspilot::CredentialStatus::SecretTooLarge);
}

TEST(CredentialStorageTests, AcceptsMaximumCredentialBlobSize)
{
    syspilot::CredentialStorage storage;
    const QString target = unique_target(QStringLiteral("maximum_blob"));
    CredentialCleanup cleanup(target);
    const QByteArray secret(2560, 's');

    const auto writeError = storage.write(target, {QStringLiteral("user"), secret});
    ASSERT_TRUE(writeError.ok())
        << "status=" << static_cast<int>(writeError.status)
        << " native_error=" << writeError.native_code;
    const auto result = storage.read(target);
    ASSERT_TRUE(result.ok());
    EXPECT_EQ(result.credential->secret, secret);
}

#else

TEST(CredentialStorageTests, ReportsUnsupportedPlatform)
{
    syspilot::CredentialStorage storage;
    const QString target = QStringLiteral("UnitedSystems.Uniter.Tests/unsupported");

    EXPECT_EQ(storage.write(target, {}).status, syspilot::CredentialStatus::UnsupportedPlatform);
    EXPECT_EQ(storage.read(target).error.status, syspilot::CredentialStatus::UnsupportedPlatform);
    EXPECT_EQ(storage.remove(target).status, syspilot::CredentialStatus::UnsupportedPlatform);
}

#endif
