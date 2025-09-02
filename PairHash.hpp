#ifndef PAIR_HASH_HPP
#define PAIR_HASH_HPP

#include <utility>
#include <functional>

template <typename T1, typename T2>
struct pair_hash {
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        std::size_t h1 = std::hash<T1>{}(p.first);
        std::size_t h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

#endif // PAIR_HASH_HPP
