#include <gtest/gtest.h>

#include <sstream>
#include <string>

#include <QDebug>

namespace {

QString capturedDebugMessage;

void captureDebugMessage(QtMsgType type, const QMessageLogContext&, const QString& message)
{
    if(type == QtDebugMsg) {
        capturedDebugMessage = message;
    }
}

class MessageHandlerGuard final {
public:
    explicit MessageHandlerGuard(QtMessageHandler handler)
        : previous_(qInstallMessageHandler(handler))
    {
    }

    ~MessageHandlerGuard()
    {
        qInstallMessageHandler(previous_);
    }

    MessageHandlerGuard(const MessageHandlerGuard&) = delete;
    MessageHandlerGuard& operator=(const MessageHandlerGuard&) = delete;

private:
    QtMessageHandler previous_;
};

} // namespace

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

TEST(LoggerTests, DuplicatesLogToQtDebugOnlyInDebugBuild)
{
    capturedDebugMessage.clear();
    const MessageHandlerGuard handlerGuard(captureDebugMessage);
    const std::string marker = "logger terminal duplication marker";

    syspilot::Logger::instance().log(marker);

#ifdef DEBUG_BUILD
    EXPECT_EQ(capturedDebugMessage, QString::fromStdString(marker));
#else
    EXPECT_TRUE(capturedDebugMessage.isEmpty());
#endif

    std::ostringstream out;
    out << syspilot::Logger::instance();
    EXPECT_NE(out.str().find(marker), std::string::npos);
}
