#pragma once

#include <string>
#include <memory>
#include <vector>
#include <mysql/mysql.h>

class Database {
public:
    static Database& getInstance();
    
    bool connect(const std::string& host, const std::string& user,
                const std::string& password, const std::string& database);
    void disconnect();
    
    bool execute(const std::string& query);
    std::vector<std::vector<std::string>> query(const std::string& query);
    
private:
    Database();
    ~Database();
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
    MYSQL* mysql_;
    bool connected_;
}; 