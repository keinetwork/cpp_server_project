#include "common/protocol.hpp"
#include <cstring>
#include <stdexcept>  // 이 줄을 추가

uint32_t Protocol::Message::sequence_counter_ = 0;

Protocol::Message::Message(MessageType type, const std::vector<uint8_t>& data)
    : data_(data)
{
    header_.version = VERSION;
    header_.type = type;
    header_.length = static_cast<uint32_t>(data.size());
    header_.sequence = ++sequence_counter_;
}

std::vector<uint8_t> Protocol::Message::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.resize(sizeof(MessageHeader) + data_.size());
    
    // 헤더 복사
    std::memcpy(buffer.data(), &header_, sizeof(MessageHeader));
    
    // 데이터 복사
    if (!data_.empty()) {
        std::memcpy(buffer.data() + sizeof(MessageHeader), data_.data(), data_.size());
    }
    
    return buffer;
}

Protocol::Message Protocol::Message::deserialize(const std::vector<uint8_t>& data) {
    if (data.size() < sizeof(MessageHeader)) {
        throw std::runtime_error("Invalid message format");
    }
    
    MessageHeader header;
    std::memcpy(&header, data.data(), sizeof(MessageHeader));
    
    if (header.version != VERSION) {
        throw std::runtime_error("Unsupported protocol version");
    }
    
    std::vector<uint8_t> message_data;
    if (header.length > 0) {
        message_data.resize(header.length);
        std::memcpy(message_data.data(), data.data() + sizeof(MessageHeader), header.length);
    }
    
    Message message(header.type, message_data);
    message.header_.sequence = header.sequence;
    
    return message;
} 