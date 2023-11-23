#pragma once

#include <array>
#include <bit>
#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <queue>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace fs = std::filesystem;

using ios = std::ios;

using u8 = std::uint8_t;
using u64 = std::uint64_t;

template <class T, class Container = std::vector<T>,
          class Compare = std::less<typename Container::value_type>>
class PriorityQueue : public std::priority_queue<T, Container, Compare> {
  public:
    T top_and_pop() {
        std::pop_heap(c.begin(), c.end(), comp);
        T value = std::move(c.back());
        c.pop_back();
        return value;
    }

  protected:
    using std::priority_queue<T, Container, Compare>::c;
    using std::priority_queue<T, Container, Compare>::comp;
};

/* Safe and saturating logical shifts */
u64 shl(u64 value, u8 count) { return count < 64 ? (value << count) : 0; }
u64 shr(u64 value, u8 count) { return count < 64 ? (value >> count) : 0; }

/// Extracts the lowest `count` bits from `value`.
u64 extract_lo(u64 value, u8 count) { return value & (shl(1, count) - 1); }
