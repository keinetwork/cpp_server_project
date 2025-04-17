#pragma once

#include <vector>
#include <mutex>
#include <memory>
#include <cstddef>

template<typename T>
class MemoryPool {
public:
    static MemoryPool& getInstance();
    
    T* allocate();
    void deallocate(T* ptr);
    void reserve(size_t count);
    
private:
    MemoryPool();
    ~MemoryPool();
    MemoryPool(const MemoryPool&) = delete;
    MemoryPool& operator=(const MemoryPool&) = delete;
    
    std::vector<std::unique_ptr<T[]>> blocks_;
    std::vector<T*> free_list_;
    std::mutex mutex_;
    static const size_t BLOCK_SIZE = 1024;
}; 