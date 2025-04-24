#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>
#include <array>
#include <thread>

class Client {
public:
    Client(boost::asio::io_context& io_context,
           const std::string& host,
           const std::string& port);

private:
    void connect(const boost::asio::ip::tcp::resolver::results_type& endpoints);
    void do_handshake();
    void start_read();
    void start_console_input();

    boost::asio::ip::tcp::socket socket_;
    boost::asio::ssl::context ssl_context_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket&> ssl_socket_;
    std::array<char, 1024> read_buffer_;
}; 