#pragma once

#include <cstdlib>

namespace simplerand {
    inline void from_seed(unsigned seed) {
        std::srand(seed);
    }

    inline float gen() {
        return rand() / float(RAND_MAX);
    }

    inline bool gen_bool(float prob) {
        return gen() < prob;
    }

    template<typename T>
    T gen_range(T min_value, T max_value) {
        return static_cast<T>(gen() * (max_value - min_value) + min_value);
    }
}