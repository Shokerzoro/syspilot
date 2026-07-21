#include "fileintegrity.h"

#include <QCryptographicHash>
#include <QFile>
#include <QString>

#include <system_error>

namespace syspilot {

bool file_sha256(const std::filesystem::path& path,
                 std::string& hash,
                 std::string& error) noexcept {
    hash.clear();
    error.clear();

    std::error_code filesystemError;
    if (!std::filesystem::is_regular_file(path, filesystemError) || filesystemError) {
        error = "Path is not a regular file: " + path.generic_string();
        return false;
    }

    QFile file(QString::fromStdString(path.string()));
    if (!file.open(QIODevice::ReadOnly)) {
        error = "Unable to open file: " + path.generic_string();
        return false;
    }

    QCryptographicHash digest(QCryptographicHash::Sha256);
    if (!digest.addData(&file)) {
        error = "Unable to read file: " + path.generic_string();
        return false;
    }

    hash = digest.result().toHex().toStdString();
    return true;
}

bool file_matches(const std::filesystem::path& path,
                  std::uint64_t expectedByteSize,
                  const std::string& expectedSha256) noexcept {
    if (expectedSha256.empty()) {
        return false;
    }

    std::error_code error;
    if (!std::filesystem::is_regular_file(path, error) || error) {
        return false;
    }
    const auto actualSize = std::filesystem::file_size(path, error);
    if (error || actualSize != expectedByteSize) {
        return false;
    }

    std::string actualHash;
    std::string hashError;
    if (!file_sha256(path, actualHash, hashError)) {
        return false;
    }

    return QString::fromStdString(actualHash).compare(
               QString::fromStdString(expectedSha256), Qt::CaseInsensitive) == 0;
}

} // namespace syspilot
