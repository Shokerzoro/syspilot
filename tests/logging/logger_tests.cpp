#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include <syspilot/verbose/logger.h>

TEST(LoggerTests, StreamOperatorWritesLoggedStrings)
{
    auto& logger = syspilot::Logger::instance();

    const std::string first = "first test log";
    const std::string second = "second test log";

    logger.log(first);
    logger.log(second);

    std::ostringstream out;
    out << logger;

    const std::string result = out.str();

    EXPECT_NE(result.find(first), std::string::npos);
    EXPECT_NE(result.find(second), std::string::npos);
}
