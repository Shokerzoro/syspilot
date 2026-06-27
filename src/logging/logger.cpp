#include "logger.h"

#include <QByteArray>
#include <QDateTime>
#include <QHostInfo>
#include <QString>
#include <QUdpSocket>

#ifndef UNITER_WEB_DOMAIN
#define UNITER_WEB_DOMAIN "localhost"
#endif

namespace syspilot {

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::log(const std::string& str)
{
    push_log(str);
}

void Logger::full_log(const std::string& str)
{
    const std::string time = QDateTime::currentDateTime().toString(Qt::ISODateWithMs).toStdString();
    push_log(time + " " + str);
}

void Logger::crash_notify()
{
    const std::string payload = make_crash_payload();

    const QHostInfo hostInfo = QHostInfo::fromName(QString::fromUtf8(UNITER_WEB_DOMAIN));
    const auto addresses = hostInfo.addresses();

    if(addresses.empty()) {
        return;
    }

    QUdpSocket socket;
    socket.writeDatagram(
        QByteArray::fromStdString(payload),
        addresses.front(),
        6666
    );
}

Logger::Logger(int length)
    : max_logs_(length)
{
}

void Logger::push_log(const std::string& str)
{
    std::lock_guard lock(mutex_);

    if(static_cast<int>(logs_.size()) >= max_logs_) {
        logs_.pop_front();
    }

    logs_.push_back(str);
}

std::string Logger::make_crash_payload() const
{
    std::lock_guard lock(mutex_);

    std::string payload;
    for(const auto& log : logs_) {
        payload += log;
        payload += '\n';
    }

    return payload;
}

} // namespace syspilot
