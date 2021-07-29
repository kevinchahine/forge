#pragma once

#include <stdint.h>

namespace forge
{
    template<typename T>
    void hash_combine(size_t& seed, const T& obj) {
        size_t k = std::hash<T>{}(obj);
        
        const uint64_t m = 0xc6a4a7935bd1e995;
        const int r = 47;

        k *= m;
        k ^= k >> r;
        k *= m;

        seed ^= k;
        seed *= m;

        // Completely arbitrary number, to prevent 0's
        // from hashing to 0.
        seed += 0xe6546b64;
    }
} // namespace forge