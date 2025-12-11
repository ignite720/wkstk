#pragma once

#include "noncopyable.hpp"

template<typename T>
class Singleton : public NonCopyable {
protected:
    Singleton() = default;

public:
    static void create_instance() {
        s_instance = new T();
    }
    
    static void destroy_instance() {
        drop(s_instance);
    }
    
    static T & get_instance() {
        return *s_instance;
    }

private:
    static T *s_instance;
};

template<typename T>
T *Singleton<T>::s_instance = nullptr;