cmake_minimum_required(VERSION 3.10)
project(cpp_server_project VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Boost 라이브러리 찾기
find_package(Boost REQUIRED COMPONENTS system thread)

# OpenSSL 라이브러리 찾기
find_package(OpenSSL REQUIRED)

# MySQL 라이브러리 찾기
find_package(MySQL REQUIRED)

# 프로젝트 구조 설정
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# 서버 실행 파일
add_executable(server
    ${SRC_DIR}/server/main.cpp
    ${SRC_DIR}/server/server.cpp
    ${SRC_DIR}/server/session.cpp
    ${SRC_DIR}/common/logger.cpp
    ${SRC_DIR}/common/protocol.cpp
    ${SRC_DIR}/common/database.cpp
    ${SRC_DIR}/common/ssl_context.cpp
    ${SRC_DIR}/common/memory_pool.cpp
    ${SRC_DIR}/common/monitor.cpp
)

# 클라이언트 실행 파일
add_executable(client
    ${SRC_DIR}/client/main.cpp
    ${SRC_DIR}/client/client.cpp
    ${SRC_DIR}/common/logger.cpp
    ${SRC_DIR}/common/protocol.cpp
)

# 헤더 파일 경로 설정
target_include_directories(server PRIVATE ${INCLUDE_DIR})
target_include_directories(client PRIVATE ${INCLUDE_DIR})

# 라이브러리 링크
target_link_libraries(server PRIVATE 
    Boost::system 
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
    MySQL::MySQL
)

target_link_libraries(client PRIVATE 
    Boost::system 
    Boost::thread
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 설치 설정
install(TARGETS server client
    RUNTIME DESTINATION bin
) 