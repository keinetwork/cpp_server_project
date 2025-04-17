# C++ 연계서버 프로젝트

이 프로젝트는 리눅스 환경에서 동작하는 C++ 기반의 연계서버입니다.

## 기능
- TCP/IP 소켓 통신
- 멀티스레드 처리
- 비동기 I/O
- 로깅 시스템
- 설정 파일 관리

## 빌드 방법
```bash
mkdir build
cd build
cmake ..
make
```

## 실행 방법
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