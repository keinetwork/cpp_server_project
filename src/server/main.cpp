#include "server/server.hpp"
#include "common/logger.hpp"
#include <iostream>
#include <csignal>

std::unique_ptr<Server> server;

void signal_handler(int signal) {
    if (server) {
        server->stop();
    }
}

int main() {
    try {
        // 로거 초기화
        Logger::init();

        // 시그널 핸들러 설정
        std::signal(SIGINT, signal_handler);
        std::signal(SIGTERM, signal_handler);

        // 서버 생성 및 시작
        server = std::make_unique<Server>("0.0.0.0", 8080);
        server->start();

        // 메인 스레드 대기
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& e) {
        Logger::error("서버 실행 중 오류 발생: " + std::string(e.what()));
        return 1;
    }

    return 0;
} 