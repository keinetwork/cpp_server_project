#pragma once

#include <string>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void init();
    static void info(const std::string& message);
    static void error(const std::string& message);
    static void debug(const std::string& message);
    static void warn(const std::string& message);

private:
    static std::shared_ptr<spdlog::logger> logger_;
}; 