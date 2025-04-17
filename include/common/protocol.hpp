#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace Protocol {
    // 프로토콜 버전
    const uint8_t VERSION = 0x01;

    // 메시지 타입
    enum class MessageType : uint8_t {
        HEARTBEAT = 0x01,
        LOGIN = 0x02,
        LOGOUT = 0x03,
        DATA = 0x04,
        ERROR = 0xFF
    };

    // 메시지 헤더
    struct MessageHeader {
        uint8_t version;
        MessageType type;
        uint32_t length;
        uint32_t sequence;
    };

    // 메시지 클래스
    class Message {
    public:
        Message(MessageType type, const std::vector<uint8_t>& data = {});
        std::vector<uint8_t> serialize() const;
        static Message deserialize(const std::vector<uint8_t>& data);
        
        MessageType getType() const { return header_.type; }
        const std::vector<uint8_t>& getData() const { return data_; }
        uint32_t getSequence() const { return header_.sequence; }

    private:
        MessageHeader header_;
        std::vector<uint8_t> data_;
        static uint32_t sequence_counter_;
    };
} 