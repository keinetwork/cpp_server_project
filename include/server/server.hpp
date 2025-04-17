#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <thread>
#include <vector>

class Server {
public:
    Server(const std::string& address, unsigned short port);
    ~Server();

    void start();
    void stop();

private:
    void do_accept();
    void handle_accept(const boost::system::error_code& error);

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;
    std::vector<std::thread> threads_;
    bool running_;
}; 