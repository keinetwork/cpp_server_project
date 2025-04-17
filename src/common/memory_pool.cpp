#include "common/memory_pool.hpp"
#include "common/logger.hpp"

template<typename T>
MemoryPool<T>& MemoryPool<T>::getInstance() {
    static MemoryPool<T> instance;
    return instance;
}

template<typename T>
MemoryPool<T>::MemoryPool() = default;

template<typename T>
MemoryPool<T>::~MemoryPool() {
    std::lock_guard<std::mutex> lock(mutex_);
    blocks_.clear();
    free_list_.clear();
}

template<typename T>
void MemoryPool<T>::reserve(size_t count) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    size_t needed_blocks = (count + BLOCK_SIZE - 1) / BLOCK_SIZE;
    size_t current_blocks = blocks_.size();
    
    if (needed_blocks > current_blocks) {
        size_t blocks_to_add = needed_blocks - current_blocks;
        for (size_t i = 0; i < blocks_to_add; ++i) {
            auto block = std::make_unique<T[]>(BLOCK_SIZE);
            for (size_t j = 0; j < BLOCK_SIZE; ++j) {
                free_list_.push_back(&block[j]);
            }
            blocks_.push_back(std::move(block));
        }
        Logger::info("메모리 풀 확장: " + std::to_string(blocks_to_add) + " 블록 추가");
    }
}

template<typename T>
T* MemoryPool<T>::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (free_list_.empty()) {
        reserve(BLOCK_SIZE);
    }
    
    T* ptr = free_list_.back();
    free_list_.pop_back();
    return ptr;
}

template<typename T>
void MemoryPool<T>::deallocate(T* ptr) {
    if (!ptr) return;
    
    std::lock_guard<std::mutex> lock(mutex_);
    free_list_.push_back(ptr);
}

// 일반적으로 사용되는 타입에 대한 명시적 인스턴스화
template class MemoryPool<char>;
template class MemoryPool<int>;
template class MemoryPool<double>; 