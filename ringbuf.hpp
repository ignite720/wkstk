#pragma once

#include <stddef.h>
#include <assert.h>

#include <array>

template<typename T, size_t N>
class RingBuf {
public:
    static_assert(!std::is_pointer_v<T>, "T cannot be a pointer because it may cause memory leaks.");
    
    using Super = RingBuf<T, N>;
    
    struct iterator {
        explicit iterator(Super &super, size_t index) : m_super(super), m_index(index) {}
        
        iterator & operator=(const iterator &other) { m_super = other.m_super; m_index = other.m_index; return *this; }
        
        T & operator*() { return m_super[m_index]; }
        //T * operator->() { return &m_super[m_index]; }
        
        const iterator & operator++() { m_index = std::min(m_index + 1, m_super.size()); return *this; }
        iterator & operator++(int) { iterator old(*this); this->operator++(); return old; }
        
        const iterator & operator--() { m_index = std::max(m_index - 1, static_cast<size_t>(0)); return *this; }
        iterator & operator--(int) { iterator old(*this); this->operator--(); return old; }
        
        bool operator==(const iterator &other) const { return (&m_super == &other.m_super) && (m_index == other.m_index); }
        bool operator!=(const iterator &other) const { return !(*this == other); }
        
    private:
        Super &m_super;
        size_t m_index = 0;
    };
    struct reverse_iterator;
    
    iterator begin();
    iterator end();
    
    reverse_iterator rbegin();
    reverse_iterator rend();

    T & operator[](size_t index) noexcept {
        return m_data[this->wrap(index)];
    }

    const T & operator[](size_t index) const noexcept {
        return m_data[this->wrap(index)];
    }

    size_t capacity() const noexcept { return N; }
    bool empty() const noexcept { return m_size == 0; }
    size_t size() const noexcept { return m_size; }
    
    void clear() {
        
    }
    
    void push_back(const T &value) noexcept {

    }

private:
    size_t wrap(size_t index) const noexcept {
        size_t offset = (m_front_index + index);
        return offset < N ? offset : (offset - N);
    }

private:
    std::array<T, N> m_data;
    size_t m_size = 0;
    size_t m_front_index = 0;
};

template<typename T, size_t N>
struct RingBuf<T, N>::reverse_iterator {
    explicit reverse_iterator(Super &super, size_t index) : m_super(super), m_index(index) {}
        
    reverse_iterator & operator=(const reverse_iterator &other) { m_super = other.m_super; m_index = other.m_index; return *this; }
    
    T & operator*() { return m_super[m_index]; }
    //T * operator->() { return &m_super[m_index]; }
    
    const reverse_iterator & operator++() { m_index = std::min(m_index + 1, m_super.size()); return *this; }
    reverse_iterator & operator++(int) { reverse_iterator old(*this); this->operator++(); return old; }
    
    const reverse_iterator & operator--() { m_index = std::max(m_index - 1, static_cast<size_t>(0)); return *this; }
    reverse_iterator & operator--(int) { reverse_iterator old(*this); this->operator--(); return old; }
    
    bool operator==(const reverse_iterator &other) const { return (&m_super == &other.m_super) && (m_index == other.m_index); }
    bool operator!=(const reverse_iterator &other) const { return !(*this == other); }
        
private:
    Super &m_super;
    size_t m_index = 0;
};

template<typename T, size_t N>
RingBuf<T, N>::iterator RingBuf<T, N>::begin() {
    return iterator(*this, 0);
}

template<typename T, size_t N>
RingBuf<T, N>::iterator RingBuf<T, N>::end() {
    return iterator(*this, this->size());
}

template<typename T, size_t N>
RingBuf<T, N>::reverse_iterator RingBuf<T, N>::rbegin() {
    return reverse_iterator(*this, 0);
}

template<typename T, size_t N>
RingBuf<T, N>::reverse_iterator RingBuf<T, N>::rend() {
    return reverse_iterator(*this, this->size());
}