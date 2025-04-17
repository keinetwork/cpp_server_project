#include "server/session.hpp"
#include "common/logger.hpp"
#include <iostream>

Session::Session(boost::asio::ip::tcp::socket socket)
    : socket_(std::move(socket))
    , is_writing_(false)
    , is_stopped_(false)
{
    Logger::info("새로운 세션이 생성되었습니다.");
}

Session::~Session() {
    stop();
}

void Session::start() {
    do_read();
}

void Session::stop() {
    if (is_stopped_) return;
    
    is_stopped_ = true;
    boost::system::error_code ec;
    socket_.close(ec);
    
    if (ec) {
        Logger::error("소켓 종료 중 오류 발생: " + ec.message());
    }
    
    Logger::info("세션이 종료되었습니다.");
}

void Session::send(const std::string& message) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    write_queue_.push(message);
    
    if (!is_writing_) {
        do_write();
    }
}

void Session::do_read() {
    if (is_stopped_) return;
    
    socket_.async_read_some(boost::asio::buffer(buffer_),
        [self = shared_from_this()](const boost::system::error_code& error, size_t bytes_transferred) {
            self->handle_read(error, bytes_transferred);
        });
}

void Session::do_write() {
    if (is_stopped_) return;
    
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (write_queue_.empty()) {
        is_writing_ = false;
        return;
    }
    
    is_writing_ = true;
    const std::string& message = write_queue_.front();
    
    boost::asio::async_write(socket_, boost::asio::buffer(message),
        [self = shared_from_this()](const boost::system::error_code& error, size_t) {
            self->handle_write(error);
        });
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
    if (error) {
        if (error != boost::asio::error::operation_aborted) {
            Logger::error("읽기 오류 발생: " + error.message());
        }
        stop();
        return;
    }
    
    // 수신된 데이터 처리
    std::string message(buffer_.data(), bytes_transferred);
    Logger::debug("수신된 메시지: " + message);
    
    // 에코 응답
    send("Echo: " + message);
    
    // 다음 읽기 시작
    do_read();
}

void Session::handle_write(const boost::system::error_code& error) {
    if (error) {
        if (error != boost::asio::error::operation_aborted) {
            Logger::error("쓰기 오류 발생: " + error.message());
        }
        stop();
        return;
    }
    
    std::lock_guard<std::mutex> lock(queue_mutex_);
    write_queue_.pop();
    do_write();
} 