#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

// Single-producer / single-consumer ring buffer, safe between one ISR and main.
//   - push(): producer only (e.g. USART2_IRQHandler). Writes head.
//   - pop():  consumer only (main). Writes tail.
// Each index has exactly one writer, so no locking is needed. One slot is kept
// empty to distinguish full from empty: capacity is N - 1. When full, push()
// drops the NEW element and returns false (the oldest data is never overwritten).
template <class T, std::size_t N>
struct ring_buffer {
    static_assert(N >= 2, "ring_buffer needs N >= 2: one slot is always kept empty");
    static_assert(N <= UINT32_MAX, "indices are uint32_t");

    // N as the index type: std::size_t is 64-bit on the host, 32-bit on the target.
    static constexpr std::uint32_t kN = static_cast<std::uint32_t>(N);

    std::array<T, N> arr{};
    std::uint32_t volatile head = 0, tail = 0;

    constexpr ring_buffer() = default;

    bool push(T t)  // true = stored, false = full (element dropped)
    {
        const std::uint32_t next = (head + 1) % kN;
        if (next == tail) {
            return false;
        }
        arr[head] = t;
        std::atomic_signal_fence(std::memory_order_release);  // data before publish
        head = next;
        return true;
    }

    bool pop(T* out)  // true = element read, false = empty (out untouched)
    {
        if (head == tail) {
            return false;
        }
        std::atomic_signal_fence(std::memory_order_acquire);  // check before data
        *out = arr[tail];
        std::atomic_signal_fence(std::memory_order_release);  // data before handing slot back
        tail = (tail + 1) % kN;
        return true;
    }

    bool is_empty() const { return head == tail; }
    std::uint32_t get_count() const { return (head + kN - tail) % kN; }
    static constexpr std::uint32_t capacity() { return kN - 1; }
};