#pragma once

#include <cstdint>
#include <filesystem>
#include <string>

namespace syspilot {

bool file_sha256(const std::filesystem::path& path,
                 std::string& hash,
                 std::string& error) noexcept;

bool file_matches(const std::filesystem::path& path,
                  std::uint64_t expectedByteSize,
                  const std::string& expectedSha256) noexcept;

} // namespace syspilot
