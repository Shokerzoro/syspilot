#include "credentialstorage.h"

#include <utility>

#ifdef Q_OS_WIN
#include <windows.h>
#include <wincred.h>

#include <string>
#endif

namespace syspilot {
namespace {

CredentialError unsupported_platform()
{
    return {CredentialStatus::UnsupportedPlatform, 0};
}

#ifdef Q_OS_WIN
// Current Windows supports 5 * 512 bytes; bundled MinGW headers still report 512.
constexpr qsizetype maximumCredentialBlobSize = 5 * 512;

CredentialError map_windows_error(DWORD error)
{
    const auto nativeError = static_cast<quint32>(error);
    switch(error) {
    case ERROR_NOT_FOUND:
        return {CredentialStatus::NotFound, nativeError};
    case ERROR_ACCESS_DENIED:
        return {CredentialStatus::AccessDenied, nativeError};
    case ERROR_NO_SUCH_LOGON_SESSION:
        return {CredentialStatus::NoLogonSession, nativeError};
    default:
        return {CredentialStatus::OsError, nativeError};
    }
}

CredentialError validate_target(const QString& target)
{
    if(target.isEmpty()
       || target.contains(QChar(u'\0'))
       || target.size() > static_cast<qsizetype>(CRED_MAX_GENERIC_TARGET_NAME_LENGTH)) {
        return {CredentialStatus::InvalidArgument, 0};
    }

    return {};
}

CredentialError validate_credential(const Credential& credential)
{
    if(credential.username.contains(QChar(u'\0'))
       || credential.username.size() > static_cast<qsizetype>(CRED_MAX_USERNAME_LENGTH)) {
        return {CredentialStatus::InvalidArgument, 0};
    }

    if(credential.secret.size() > maximumCredentialBlobSize) {
        return {CredentialStatus::SecretTooLarge, 0};
    }

    return {};
}

class CredentialBuffer final {
public:
    CredentialBuffer() = default;

    ~CredentialBuffer()
    {
        if(value_ == nullptr) {
            return;
        }

        if(value_->CredentialBlob != nullptr && value_->CredentialBlobSize > 0) {
            SecureZeroMemory(value_->CredentialBlob, value_->CredentialBlobSize);
        }
        CredFree(value_);
    }

    CredentialBuffer(const CredentialBuffer&) = delete;
    CredentialBuffer(CredentialBuffer&&) = delete;
    CredentialBuffer& operator=(const CredentialBuffer&) = delete;
    CredentialBuffer& operator=(CredentialBuffer&&) = delete;

    PCREDENTIALW* output() noexcept { return &value_; }
    PCREDENTIALW get() const noexcept { return value_; }

private:
    PCREDENTIALW value_ = nullptr;
};

#endif

} // namespace

CredentialError CredentialStorage::write(
    const QString& target,
    const Credential& storedCredential) const
{
#ifdef Q_OS_WIN
    const CredentialError targetError = validate_target(target);
    if(!targetError.ok()) {
        return targetError;
    }

    const CredentialError credentialError = validate_credential(storedCredential);
    if(!credentialError.ok()) {
        return credentialError;
    }

    std::wstring nativeTarget = target.toStdWString();
    std::wstring nativeUsername = storedCredential.username.toStdWString();

    CREDENTIALW credential{};
    credential.Type = CRED_TYPE_GENERIC;
    credential.TargetName = nativeTarget.data();
    credential.CredentialBlobSize = static_cast<DWORD>(storedCredential.secret.size());
    credential.CredentialBlob = storedCredential.secret.isEmpty()
        ? nullptr
        : reinterpret_cast<LPBYTE>(const_cast<char*>(storedCredential.secret.constData()));
    credential.Persist = CRED_PERSIST_LOCAL_MACHINE;
    credential.UserName = nativeUsername.empty() ? nullptr : nativeUsername.data();

    if(CredWriteW(&credential, 0) == FALSE) {
        return map_windows_error(GetLastError());
    }

    return {};
#else
    Q_UNUSED(target);
    Q_UNUSED(storedCredential);
    return unsupported_platform();
#endif
}

CredentialReadResult CredentialStorage::read(const QString& target) const
{
#ifdef Q_OS_WIN
    const CredentialError targetError = validate_target(target);
    if(!targetError.ok()) {
        return {std::nullopt, targetError};
    }

    const std::wstring nativeTarget = target.toStdWString();
    CredentialBuffer buffer;
    if(CredReadW(nativeTarget.c_str(), CRED_TYPE_GENERIC, 0, buffer.output()) == FALSE) {
        return {std::nullopt, map_windows_error(GetLastError())};
    }

    const PCREDENTIALW nativeCredential = buffer.get();
    Credential credential;
    if(nativeCredential->UserName != nullptr) {
        credential.username = QString::fromWCharArray(nativeCredential->UserName);
    }
    if(nativeCredential->CredentialBlob != nullptr && nativeCredential->CredentialBlobSize > 0) {
        credential.secret = QByteArray(
            reinterpret_cast<const char*>(nativeCredential->CredentialBlob),
            static_cast<qsizetype>(nativeCredential->CredentialBlobSize));
    }

    return {std::move(credential), {}};
#else
    Q_UNUSED(target);
    return {std::nullopt, unsupported_platform()};
#endif
}

CredentialError CredentialStorage::remove(const QString& target) const
{
#ifdef Q_OS_WIN
    const CredentialError targetError = validate_target(target);
    if(!targetError.ok()) {
        return targetError;
    }

    const std::wstring nativeTarget = target.toStdWString();
    if(CredDeleteW(nativeTarget.c_str(), CRED_TYPE_GENERIC, 0) == FALSE) {
        return map_windows_error(GetLastError());
    }

    return {};
#else
    Q_UNUSED(target);
    return unsupported_platform();
#endif
}

} // namespace syspilot
