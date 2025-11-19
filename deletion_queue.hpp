#pragma once

#include <functional>
#include <map>

namespace wkstk {
    class DeletionQueue {
    public:
        using Priority = size_t;
        using Deletor = std::function<void()>;
        
        static constexpr Priority MIN_PRIORITY = 0;
        static constexpr Priority MAX_PRIORITY = -1;
        
        ~DeletionQueue() {
            this->flush();
        }

        void push(Deletor &&value, Priority priority = MIN_PRIORITY) {
            m_deletors.emplace(priority, std::move(value));
        }

        void flush() {
            for (auto iter = m_deletors.rbegin(); iter != m_deletors.rend(); ++iter) {
                iter->second();
            }
            m_deletors.clear();
        }

    private:
        std::multimap<Priority, Deletor> m_deletors;
    };
}