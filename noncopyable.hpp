#pragma once

class NonCopyable {
protected:
    NonCopyable() = default;

public:
    virtual ~NonCopyable() = default;

private:
    NonCopyable(const NonCopyable &other) = delete;
    T & operator=(const NonCopyable &other) = delete;
};