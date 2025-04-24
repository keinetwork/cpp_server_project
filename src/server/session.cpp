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
    
    try {
        // 수신된 데이터를 Protocol::Message로 변환
        std::vector<uint8_t> received_data(buffer_.data(), buffer_.data() + bytes_transferred);
        auto message = Protocol::Message::deserialize(received_data);
        
        // 메시지 타입에 따른 처리
        switch (message.getType()) {
            case Protocol::MessageType::DATA: {
                std::string data_str(message.getData().begin(), message.getData().end());
                Logger::info("수신된 메시지: " + data_str);
                
                // 에코 응답 생성
                std::string response = "Echo: " + data_str;
                std::vector<uint8_t> response_data(response.begin(), response.end());
                Protocol::Message response_msg(Protocol::MessageType::DATA, response_data);
                
                // 응답 전송
                auto serialized = response_msg.serialize();
                send(std::string(serialized.begin(), serialized.end()));
                break;
            }
            default:
                Logger::warn("알 수 없는 메시지 타입: " + std::to_string(static_cast<int>(message.getType())));
                break;
        }
    }
    catch (const std::exception& e) {
        Logger::error("메시지 처리 중 오류 발생: " + std::string(e.what()));
    }
    
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