#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <queue>
#include <mutex>

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::ip::tcp::socket socket);
    ~Session();

    void start();
    void stop();
    void send(const std::string& message);

private:
    void do_read();
    void do_write();
    void handle_read(const boost::system::error_code& error, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& error);

    boost::asio::ip::tcp::socket socket_;
    std::array<char, 8192> buffer_;
    std::queue<std::string> write_queue_;
    std::mutex queue_mutex_;
    bool is_writing_;
    bool is_stopped_;
}; 