#pragma once

#include <boost/asio/ssl.hpp>
#include <string>
#include <memory>

class SSLContext {
public:
    static SSLContext& getInstance();
    
    bool initialize(const std::string& cert_file, const std::string& key_file);
    boost::asio::ssl::context& getContext();
    
private:
    SSLContext();
    ~SSLContext();
    SSLContext(const SSLContext&) = delete;
    SSLContext& operator=(const SSLContext&) = delete;
    
    std::unique_ptr<boost::asio::ssl::context> context_;
    bool initialized_;
}; 