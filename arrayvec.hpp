#pragma once

#include <stddef.h>
#include <assert.h>

#include <array>
#include <memory>

namespace __arrayvec_details {
    template <typename T>
    struct is_shared_ptr : std::false_type {};
    
    template <typename T>
    struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
    
    template <typename T>
    struct is_unique_ptr : std::false_type {};
    
    template <typename T>
    struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
}

template<typename T, size_t N, typename Super = std::array<T, N>>
class ArrayVec : private Super {
public:
    static_assert(!__arrayvec_details::is_shared_ptr<T>::value && !__arrayvec_details::is_unique_ptr<T>::value, "T cannot be shared_ptr or unique_ptr.");
    
    using typename Super::size_type;
    using typename Super::reference;
    using typename Super::const_reference;
    using typename Super::iterator;
    using typename Super::const_iterator;
    using typename Super::reverse_iterator;
    using typename Super::const_reverse_iterator;

    using Super::at;
    using Super::data;
    using Super::front;
    
    using Super::begin;
    using Super::cbegin;

    using Super::rend;
    using Super::crend;
    
    ArrayVec() = default;
    ArrayVec(std::initializer_list<T> il) {
        for (auto i : il) {
            this->push_back(i);
        }
    }

    reference operator[](size_type index) noexcept {
        assert(index < m_size);
        return Super::operator[](index);
    }

    const_reference operator[](size_type index) const noexcept {
        assert(index < m_size);
        return Super::operator[](index);
    }

    size_type capacity() const noexcept { return N; }
    bool empty() const noexcept { return m_size == 0; }
    size_type size() const noexcept { return m_size; }

    reference back() noexcept {
        auto tmp = this->end();
        --tmp;
        return *tmp;
    }

    const_reference back() const noexcept {
        auto tmp = this->cend();
        --tmp;
        return *tmp;
    }

    iterator end() noexcept {
        return this->begin() + m_size;
    }

    const_iterator end() const noexcept {
        return this->cend();
    }

    const_iterator cend() const noexcept {
        return this->cbegin() + m_size;
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(this->end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(this->end());
    }

    void push_back(const T &value) noexcept {
        assert(m_size < N);
        *(this->data() + m_size) = value;
        ++m_size;
    }

    void pop_back() noexcept {
        --m_size;
    }

    void resize(size_type new_size) noexcept {
        assert(new_size <= N);
        m_size = new_size;
    }

    void clear() noexcept {
        m_size = 0;
    }

private:
    size_type m_size = 0;
};