# C++ 연계서버 프로젝트

이 프로젝트는 리눅스 환경에서 동작하는 C++ 기반의 연계서버입니다.

## 특징징
1.Boost.Asio를 사용한 비동기 네트워크 통신
2.멀티스레드 지원
3.로깅 시스템 (spdlog 사용)
4.시그널 핸들링
5.CMake 빌드 시스템

1. 필요한 의존성설치
```bash
sudo apt-get update
sudo apt-get install libboost-all-dev
sudo apt-get install libspdlog-dev
sudo apt-get install libssl-dev
sudo apt-get install libmysqlclient-dev
sudo apt install pkg-config0
sudo apt install libfmt-dev
```
2. 빌드
```bash
mkdir build
cd build
cmake ..
make
```
3.실행
```bash
# 서버 실행
./server

# 클라이언트 실행
./client
```

## 의존성
- C++17 이상
- CMake 3.10 이상
- Boost 라이브러리
- spdlog (로깅)
- nlohmann/json (설정 파일)

## 프로젝트 구조
```
.
├── CMakeLists.txt
├── README.md
├── include/
│   ├── common/
│   ├── server/
│   └── client/
├── src/
│   ├── server/
│   └── client/
└── config/
    └── config.json
``` 

1.클라이언트 세션 관리
 - 각 클라이언트 연결을 독립적인 세션으로 관리
 - 비동기 읽기/쓰기 처리
 - 세션 상태 관리
2.프로토콜 구현
 - 바이너리 프로토콜 정의
 - 메시지 직렬화/역직렬화
 - 시퀀스 번호 관리
3.데이터베이스 연동
 - MySQL 데이터베이스 연결 관리
 - 쿼리 실행 및 결과 처리
 - 연결 풀링
4.SSL/TLS 지원
 - OpenSSL을 이용한 보안 통신
 - 인증서 관리
 - 암호화 통신
5.메모리 풀 관리
 - 객체 재사용을 통한 메모리 효율성 향상
 - 스레드 안전한 메모리 할당/해제
 - 동적 확장 지원
6.성능 모니터링
 - 실행 시간 측정
 - 카운터 및 게이지 메트릭
 - 성능 통계 수집

 추가기능 필요 요소
1. SSL/TLS 설정:
 - 인증서와 개인키 파일 준비
 - SSLContext 초기화
2.데이터베이스 설정:
 - MySQL 서버 접속 정보 설정
 - 필요한 테이블 생성
3.성능 모니터링:
 - 모니터링할 지표 정의
 - 주기적인 메트릭 수집