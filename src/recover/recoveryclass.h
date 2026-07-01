#pragma once

#include <filesystem>

namespace syspilot {

class RecoveryClass final {
public:
    void setdata(const std::filesystem::path& appDir, const std::filesystem::path& tempDir);
    void prepare(const std::filesystem::path& appDir, const std::filesystem::path& tempDir);
    void relax();
    void recover();

private:
    void clear_state();

private:
    std::filesystem::path appDir_;
    std::filesystem::path tempDir_;
    std::filesystem::path recoverDir_;
    bool prepared_ = false;
};

} // namespace syspilot