#pragma once

#include <deque>
#include <mutex>
#include <ostream>
#include <string>

namespace syspilot {

class Logger final {
public:
    // Public API
    static Logger& instance();

    void log(const std::string& str);
    void full_log(const std::string& str);
    void crash_notify();

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger& operator=(Logger&&) = delete;

    friend std::ostream& operator<<(std::ostream& os, const Logger& logger);

private:
    explicit Logger(int length = 124);

    void push_log(const std::string& str);
    std::string make_crash_payload() const;

private:
    // Private data
    int max_logs_;
    std::deque<std::string> logs_;
    mutable std::mutex mutex_;
};

} // namespace syspilot
