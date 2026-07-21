#include "restart.h"

#include <logging/logger.h>
#include <process/process.h>

#include <string>

namespace syspilot {

bool restart_app(qint64 appPid)
{
    auto& logger = Logger::instance();
    ProcessHandle appProcess(appPid);

    logger.full_log("restart_app(): waiting for main app process, pid=" + std::to_string(appPid));
    if(!appProcess.wait()) {
        logger.full_log("restart_app(): unable to wait for main app process, pid=" + std::to_string(appPid));
        return false;
    }

    ChildProcess app(BinType::App);
    if(!app.run()) {
        logger.full_log("restart_app(): unable to start main app process, error="
                        + app.error_string().toStdString());
        return false;
    }

    if(!app.wait_started()) {
        logger.full_log("restart_app(): main app process did not confirm startup, error="
                        + app.error_string().toStdString());
        return false;
    }

    return true;
}

} // namespace syspilot
