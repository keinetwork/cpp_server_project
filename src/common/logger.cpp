#include "common/logger.hpp"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <filesystem>
#include <iostream>

std::shared_ptr<spdlog::logger> Logger::logger_;

void Logger::init() {
    try {
        // 로그 디렉토리 생성
        std::filesystem::create_directories("logs");

        // 콘솔과 파일 로거 설정
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/server.log", 1024 * 1024 * 5, 3);

        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
        logger_ = std::make_shared<spdlog::logger>("server", sinks.begin(), sinks.end());
        
        // 로그 레벨 설정
        logger_->set_level(spdlog::level::debug);
        logger_->flush_on(spdlog::level::info);
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "로거 초기화 실패: " << ex.what() << std::endl;
    }
}

void Logger::info(const std::string& message) {
    if (logger_) {
        logger_->info(message);
    }
}

void Logger::error(const std::string& message) {
    if (logger_) {
        logger_->error(message);
    }
}

void Logger::debug(const std::string& message) {
    if (logger_) {
        logger_->debug(message);
    }
}

void Logger::warn(const std::string& message) {
    if (logger_) {
        logger_->warn(message);
    }
} 