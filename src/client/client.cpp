#include "client/client.hpp"
#include "common/logger.hpp"
#include "common/protocol.hpp"
#include <iostream>

Client::Client(boost::asio::io_context& io_context,
               const std::string& host,
               const std::string& port)
    : socket_(io_context),
      ssl_context_(boost::asio::ssl::context::tlsv12_client),
      ssl_socket_(socket_, ssl_context_) {
    
    ssl_context_.set_verify_mode(boost::asio::ssl::verify_peer);
    
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, port);
    
    connect(endpoints);
}

void Client::connect(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(
        socket_,
        endpoints,
        [this](const boost::system::error_code& error,
               const boost::asio::ip::tcp::endpoint& endpoint) {
            if (!error) {
                Logger::info("Connected to server");
                do_handshake();
            } else {
                Logger::error("Connection failed: " + std::string(error.message()));
            }
        });
}

void Client::do_handshake() {
    ssl_socket_.async_handshake(
        boost::asio::ssl::stream_base::client,
        [this](const boost::system::error_code& error) {
            if (!error) {
                Logger::info("SSL handshake completed");
                start_read();
                start_console_input();
            } else {
                Logger::error("SSL handshake failed: " + std::string(error.message()));
            }
        });
}

void Client::start_read() {
    boost::asio::async_read(
        ssl_socket_,
        boost::asio::buffer(read_buffer_),
        boost::asio::transfer_at_least(1),
        [this](const boost::system::error_code& error, std::size_t length) {
            if (!error) {
                std::string received_data(read_buffer_.begin(), read_buffer_.begin() + length);
                std::cout << "Received: " << received_data << std::endl;
                start_read();
            } else {
                Logger::error("Read failed: " + std::string(error.message()));
            }
        });
}

void Client::start_console_input() {
    std::thread([this]() {
        std::string line;
        while (std::getline(std::cin, line)) {
            // DATA 타입으로 메시지 생성
            std::vector<uint8_t> data(line.begin(), line.end());
            Protocol::Message msg(Protocol::MessageType::DATA, data);
            
            auto serialized = msg.serialize();
            boost::asio::async_write(
                ssl_socket_,
                boost::asio::buffer(serialized),
                [this](const boost::system::error_code& error, std::size_t length) {
                    if (error) {
                        Logger::error("Write failed: " + std::string(error.message()));
                    }
                });
        }
    }).detach();
}