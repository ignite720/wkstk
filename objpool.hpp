#pragma once

#include "singleton.hpp"

template<typename T>
class ObjectPool : public Singleton<ObjectPool<T>> {
    friend Singleton<ObjectPool>;

public:
    static constexpr size_t CHUNK_INITIAL_CAPACITY = 10;

    struct MemoryChunk {
        T *ptr;
        size_t num_blocks;
    };
    
    struct Stats {
        size_t total_blocks;
        size_t free_blocks;
        size_t used_blocks;
    };

private:
    ObjectPool() {
        self->clear();
    }

public:
    ~ObjectPool() {
        self->clear();
    }
    
    const Stats & stats() const {
        m_stats.total_blocks = std::accumulate(m_chunks.begin(), m_chunks.end(), as_(0, size_t), [](size_t total, const MemoryChunk &chunk) {
            return total + chunk.num_blocks;
        });
        m_stats.free_blocks = m_free_blocks.size();
        m_stats.used_blocks = (self->total_blocks() - self->free_blocks());
        //return format_("total chunks: %zu, total blocks: %zu, free blocks: %zu, used blocks: %zu", m_chunks.size(), self->total_blocks(), self->free_blocks(), self->used_blocks());
        return m_stats;
    }

    void reserve(size_t value) {
        m_next_chunk_capacity = value;
    }

    void clear() {
        for_ref *ptr in_(m_free_blocks) {
            ptr->T::~T();
        }
        m_free_blocks.clear();

        for_ref &chunk in_(m_chunks) {
            free(chunk.ptr);
        }
        m_chunks.clear();

        m_next_chunk_capacity = CHUNK_INITIAL_CAPACITY;
    }

    template<typename ...Args>
    T * allocate(Args &&...args) {
        if (m_free_blocks.empty()) {
            if (!self->allocate_chunk(m_next_chunk_capacity)) {
                return nullptr;
            }
        }

        T *ptr = m_free_blocks.back();
        m_free_blocks.pop_back();

        #if defined(WKSTK_CONFIG_MEMORY_POOL_ENABLE_ZEROED) && (WKSTK_CONFIG_MEMORY_POOL_POST_ZEROED == 0)
        memset(ptr, 0, sizeof(T));
        #endif
        new (ptr) T(std::forward<Args>(args)...);
        return ptr;
    }

    void deallocate(T *&ptr) {
        ptr->T::~T();

        #if defined(WKSTK_CONFIG_MEMORY_POOL_ENABLE_ZEROED) && (WKSTK_CONFIG_MEMORY_POOL_POST_ZEROED == 1)
        memset(ptr, 0, sizeof(T));
        #endif
        m_free_blocks.push_back(ptr);

        ptr = nullptr;
    }

private:
    bool allocate_chunk(size_t n) {
        T *ptr = as_(malloc(n * sizeof(T)), T *);
        if (!ptr) {
            return false;
        }

        #if defined(WKSTK_CONFIG_MEMORY_POOL_ENABLE_ZEROED) && (WKSTK_CONFIG_MEMORY_POOL_POST_ZEROED == 1)
        WKSTK_MEMORY_ZERO(ptr, n * sizeof(T));
        #endif
        for (size_t i = 0; i < n; ++i) {
            m_free_blocks.push_back(&ptr[i]);
        }

        m_chunks.push_back(MemoryChunk { ptr, n });
        m_next_chunk_capacity += n;
        return true;
    }

private:
    std::vector<MemoryChunk> m_chunks;
    std::vector<T *> m_free_blocks;
    size_t m_next_chunk_capacity = 0;
    Stats m_stats;
};