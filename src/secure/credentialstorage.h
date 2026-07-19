#pragma once

#include <QByteArray>
#include <QString>
#include <QtGlobal>

#include <optional>

namespace syspilot {

enum class CredentialStatus {
    Success,
    NotFound,
    InvalidArgument,
    SecretTooLarge,
    AccessDenied,
    NoLogonSession,
    UnsupportedPlatform,
    OsError,
};

struct CredentialError {
    CredentialStatus status = CredentialStatus::Success;
    quint32 native_code = 0;

    [[nodiscard]] bool ok() const noexcept { return status == CredentialStatus::Success; }
};

struct Credential {
    QString username;
    QByteArray secret;
};

struct CredentialReadResult {
    std::optional<Credential> credential;
    CredentialError error;

    [[nodiscard]] bool ok() const noexcept { return error.ok() && credential.has_value(); }
};

class CredentialStorage final {
public:
    [[nodiscard]] CredentialError write(
        const QString& target,
        const Credential& credential) const;

    [[nodiscard]] CredentialReadResult read(const QString& target) const;
    [[nodiscard]] CredentialError remove(const QString& target) const;
};

} // namespace syspilot
