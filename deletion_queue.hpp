#pragma once

#include <deque>
#include <functional>

class DeletionQueue {
public:
    using Deletor = std::function<void()>;

    ~DeletionQueue() {
        this->flush();
    }

    void push(Deletor &&value) {
        m_deletors.push_back(std::move(value));
    }

    void flush() {
        for (auto iter = m_deletors.rbegin(); iter != m_deletors.rend(); ++iter) {
            (*iter)();
        }
        m_deletors.clear();
    }

private:
    std::deque<Deletor> m_deletors;
};