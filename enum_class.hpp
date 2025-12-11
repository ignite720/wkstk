#pragma once

#define WKSTK_ENABLE_ENUM_CLASS_FLAG_BITS(T) \
template<> \
struct EnableEnumClassFlagBits<T> { \
    static constexpr bool ENABLED = true; \
};

template<typename T>
constexpr auto v(T value) {
    return static_cast<std::underlying_type_t<T>>(value);
}

template<typename T>
constexpr auto e(std::underlying_type_t<T> value) {
    return static_cast<T>(value);
}

template<typename T>
struct EnableEnumClassFlagBits {
    static constexpr bool ENABLED = false;
};

template<typename T>
std::enable_if_t<EnableEnumClassFlagBits<T>::ENABLED, T> operator~(T value) {
    return e<T>(~v(value));
}

template<typename T>
std::enable_if_t<EnableEnumClassFlagBits<T>::ENABLED, T> operator|(T lhs, T rhs) {
    return e<T>(v(lhs) | v(rhs));
}

template<typename T>
std::enable_if_t<EnableEnumClassFlagBits<T>::ENABLED, T> operator&(T lhs, T rhs) {
    return e<T>(v(lhs) & v(rhs));
}

template<typename T>
std::enable_if_t<EnableEnumClassFlagBits<T>::ENABLED, void> operator|=(T &lhs, T rhs) {
    lhs = e<T>(v(lhs) | v(rhs));
}

template<typename T>
std::enable_if_t<EnableEnumClassFlagBits<T>::ENABLED, void> operator&=(T &lhs, T rhs) {
    lhs = e<T>(v(lhs) & v(rhs));
}