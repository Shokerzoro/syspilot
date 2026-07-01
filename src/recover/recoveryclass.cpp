#include "recoveryclass.h"

#include <logging/logger.h>

#include <stdexcept>
#include <system_error>

namespace syspilot {

namespace {

std::string path_string(const std::filesystem::path& path)
{
    return path.generic_string();
}

void log_action(const std::string& explanation)
{
    Logger::instance().full_log("RecoveryClass::" + explanation);
}

void throw_if_error(const std::error_code& error,
                    const std::string& explanation,
                    const std::filesystem::path& path)
{
    if(error) {
        throw std::filesystem::filesystem_error(explanation, path, error);
    }
}

void copy_directory_content(const std::filesystem::path& source, const std::filesystem::path& target)
{
    std::error_code error;
    std::filesystem::create_directories(target, error);
    throw_if_error(error, "RecoveryClass: unable to create target directory", target);

    for(const auto& entry : std::filesystem::directory_iterator(source, error)) {
        throw_if_error(error, "RecoveryClass: unable to iterate source directory", source);

        const std::filesystem::path destination = target / entry.path().filename();
        std::filesystem::copy(
            entry.path(),
            destination,
            std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing,
            error);
        throw_if_error(error, "RecoveryClass: unable to copy recovery entry", entry.path());
    }
}

} // namespace

void RecoveryClass::setdata(const std::filesystem::path& appDir, const std::filesystem::path& tempDir)
{
    appDir_ = appDir;
    tempDir_ = tempDir;
    recoverDir_ = tempDir_ / "recover";
    prepared_ = true;

    log_action("setdata(): recovery data assigned, appDir=" + path_string(appDir_)
               + ", tempDir=" + path_string(tempDir_) + ", recoverDir=" + path_string(recoverDir_));
}

void RecoveryClass::prepare(const std::filesystem::path& appDir, const std::filesystem::path& tempDir)
{
    setdata(appDir, tempDir);
    prepared_ = false;

    log_action("prepare(): preparing recovery backup, appDir=" + path_string(appDir_)
               + ", tempDir=" + path_string(tempDir_) + ", recoverDir=" + path_string(recoverDir_));

    std::error_code error;
    if(!std::filesystem::exists(appDir_, error) || error) {
        throw std::runtime_error("RecoveryClass::prepare(): appDir does not exist, appDir=" + path_string(appDir_));
    }

    if(!std::filesystem::is_directory(appDir_, error) || error) {
        throw std::runtime_error("RecoveryClass::prepare(): appDir is not a directory, appDir=" + path_string(appDir_));
    }

    std::filesystem::remove_all(recoverDir_, error);
    throw_if_error(error, "RecoveryClass::prepare(): unable to clear recovery directory", recoverDir_);

    copy_directory_content(appDir_, recoverDir_);
    prepared_ = true;
}

void RecoveryClass::relax()
{
    if(!prepared_) {
        return;
    }

    log_action("relax(): deleting recovery backup, recoverDir=" + path_string(recoverDir_));

    std::error_code error;
    std::filesystem::remove_all(recoverDir_, error);
    throw_if_error(error, "RecoveryClass::relax(): unable to delete recovery directory", recoverDir_);
    clear_state();
}

void RecoveryClass::recover()
{
    if(!prepared_) {
        return;
    }

    log_action("recover(): restoring app directory, appDir=" + path_string(appDir_)
               + ", recoverDir=" + path_string(recoverDir_));

    std::error_code error;
    if(!std::filesystem::exists(recoverDir_, error) || error) {
        throw std::runtime_error("RecoveryClass::recover(): recoverDir does not exist, recoverDir="
                                 + path_string(recoverDir_));
    }

    std::filesystem::remove_all(appDir_, error);
    throw_if_error(error, "RecoveryClass::recover(): unable to clear app directory", appDir_);

    copy_directory_content(recoverDir_, appDir_);

    std::filesystem::remove_all(recoverDir_, error);
    throw_if_error(error, "RecoveryClass::recover(): unable to delete recovery directory", recoverDir_);
    clear_state();
}

void RecoveryClass::clear_state()
{
    appDir_.clear();
    tempDir_.clear();
    recoverDir_.clear();
    prepared_ = false;
}

} // namespace syspilot