#include "server/server.hpp"
#include "common/logger.hpp"
#include <iostream>

Server::Server(const std::string& address, unsigned short port)
    : io_context_()
    , acceptor_(io_context_, boost::asio::ip::tcp::endpoint(
        boost::asio::ip::make_address(address), port))
    , socket_(io_context_)
    , running_(false)
{
    Logger::info("서버가 초기화되었습니다. 주소: " + address + ", 포트: " + std::to_string(port));
}

Server::~Server() {
    stop();
}

void Server::start() {
    if (running_) return;
    
    running_ = true;
    do_accept();

    // 스레드 풀 생성
    unsigned int thread_count = std::thread::hardware_concurrency();
    for (unsigned int i = 0; i < thread_count; ++i) {
        threads_.emplace_back([this]() {
            io_context_.run();
        });
    }

    Logger::info("서버가 시작되었습니다. 스레드 수: " + std::to_string(thread_count));
}

void Server::stop() {
    if (!running_) return;

    running_ = false;
    io_context_.stop();

    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    threads_.clear();

    Logger::info("서버가 중지되었습니다.");
}

void Server::do_accept() {
    acceptor_.async_accept(socket_,
        [this](const boost::system::error_code& error) {
            handle_accept(error);
        });
}

void Server::handle_accept(const boost::system::error_code& error) {
    if (!error) {
        Logger::info("새로운 클라이언트가 연결되었습니다.");
        // 여기에 클라이언트 세션 처리 로직 추가
    }

    if (running_) {
        do_accept();
    }
} 