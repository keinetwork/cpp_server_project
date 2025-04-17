#include "common/database.hpp"
#include "common/logger.hpp"
#include <stdexcept>

Database& Database::getInstance() {
    static Database instance;
    return instance;
}

Database::Database() : mysql_(nullptr), connected_(false) {
    mysql_ = mysql_init(nullptr);
    if (!mysql_) {
        throw std::runtime_error("MySQL 초기화 실패");
    }
}

Database::~Database() {
    disconnect();
    if (mysql_) {
        mysql_close(mysql_);
    }
}

bool Database::connect(const std::string& host, const std::string& user,
                      const std::string& password, const std::string& database) {
    if (connected_) {
        return true;
    }
    
    if (!mysql_real_connect(mysql_, host.c_str(), user.c_str(),
                          password.c_str(), database.c_str(), 0, nullptr, 0)) {
        Logger::error("데이터베이스 연결 실패: " + std::string(mysql_error(mysql_)));
        return false;
    }
    
    connected_ = true;
    Logger::info("데이터베이스 연결 성공");
    return true;
}

void Database::disconnect() {
    if (connected_) {
        mysql_close(mysql_);
        mysql_ = mysql_init(nullptr);
        connected_ = false;
        Logger::info("데이터베이스 연결 종료");
    }
}

bool Database::execute(const std::string& query) {
    if (!connected_) {
        Logger::error("데이터베이스가 연결되지 않았습니다");
        return false;
    }
    
    if (mysql_query(mysql_, query.c_str()) != 0) {
        Logger::error("쿼리 실행 실패: " + std::string(mysql_error(mysql_)));
        return false;
    }
    
    return true;
}

std::vector<std::vector<std::string>> Database::query(const std::string& query) {
    std::vector<std::vector<std::string>> results;
    
    if (!connected_) {
        Logger::error("데이터베이스가 연결되지 않았습니다");
        return results;
    }
    
    if (mysql_query(mysql_, query.c_str()) != 0) {
        Logger::error("쿼리 실행 실패: " + std::string(mysql_error(mysql_)));
        return results;
    }
    
    MYSQL_RES* result = mysql_store_result(mysql_);
    if (!result) {
        return results;
    }
    
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    
    while ((row = mysql_fetch_row(result))) {
        std::vector<std::string> row_data;
        for (int i = 0; i < num_fields; i++) {
            row_data.push_back(row[i] ? row[i] : "NULL");
        }
        results.push_back(row_data);
    }
    
    mysql_free_result(result);
    return results;
} 