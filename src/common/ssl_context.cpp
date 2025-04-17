#include "common/ssl_context.hpp"
#include "common/logger.hpp"
#include <stdexcept>

SSLContext& SSLContext::getInstance() {
    static SSLContext instance;
    return instance;
}

SSLContext::SSLContext() : initialized_(false) {
    context_ = std::make_unique<boost::asio::ssl::context>(boost::asio::ssl::context::tls_server);
}

SSLContext::~SSLContext() = default;

bool SSLContext::initialize(const std::string& cert_file, const std::string& key_file) {
    if (initialized_) {
        return true;
    }
    
    try {
        context_->set_options(
            boost::asio::ssl::context::default_workarounds |
            boost::asio::ssl::context::no_sslv2 |
            boost::asio::ssl::context::single_dh_use);
        
        context_->use_certificate_chain_file(cert_file);
        context_->use_private_key_file(key_file, boost::asio::ssl::context::pem);
        
        initialized_ = true;
        Logger::info("SSL 컨텍스트 초기화 성공");
        return true;
    }
    catch (const std::exception& e) {
        Logger::error("SSL 컨텍스트 초기화 실패: " + std::string(e.what()));
        return false;
    }
}

boost::asio::ssl::context& SSLContext::getContext() {
    if (!initialized_) {
        throw std::runtime_error("SSL 컨텍스트가 초기화되지 않았습니다");
    }
    return *context_;
} 