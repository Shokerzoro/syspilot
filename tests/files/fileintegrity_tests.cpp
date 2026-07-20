#include <gtest/gtest.h>

#include <files/files.h>

#include <QCryptographicHash>
#include <QFile>
#include <QTemporaryDir>

#include <filesystem>

namespace {

TEST(FileIntegrityTests, MatchesSizeAndSha256) {
    QTemporaryDir directory;
    ASSERT_TRUE(directory.isValid());
    const auto path = std::filesystem::path(directory.path().toStdString()) / "payload.bin";

    QFile file(QString::fromStdString(path.string()));
    ASSERT_TRUE(file.open(QIODevice::WriteOnly));
    ASSERT_EQ(file.write("payload", 7), 7);
    file.close();

    const auto hash = QCryptographicHash::hash(
        QByteArrayLiteral("payload"), QCryptographicHash::Sha256).toHex().toStdString();
    EXPECT_TRUE(syspilot::file_matches(path, 7, hash));
    EXPECT_FALSE(syspilot::file_matches(path, 6, hash));
    EXPECT_FALSE(syspilot::file_matches(path, 7, "bad-hash"));
    EXPECT_FALSE(syspilot::file_matches(path.parent_path(), 7, hash));
    EXPECT_FALSE(syspilot::file_matches(path.parent_path() / "missing", 7, hash));
}

} // namespace
