#include <array>
#include <cstdint>

template <class T, std::size_t N> struct ring_buffer{
    std::array<T, N> arr{};
    std::uint8_t write_idx = 0, read_idx = 0, count = 0;

    constexpr ring_buffer() = default;

    void push(T t){
        arr[write_idx] = t;
        write_idx = (write_idx + 1) % N;
        if(count < N){
            count++;
        }else{
            read_idx = (read_idx + 1) % N;
        }
    }
    
    bool pop(T* out){
        if (count == 0){
            return false;          // nothing written, out untouched
        }
        *out = arr[read_idx];
        read_idx = (read_idx + 1) % N;
        --count;
        return true;
    }
    bool is_empty() const{ return count == 0; }
    std::uint8_t get_count() const{ return count; }
};



ring_buffer<uint32_t, 16> rb;      // NOT static — needs external linkage, like the C functions

void kick(){                        // NOT static — always emitted, forces everything it calls
    rb.push(1);
    uint32_t out;
    rb.pop(&out);
    rb.is_empty();
    rb.get_count();
}