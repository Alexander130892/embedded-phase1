
#include <cstdio>
#include <cstdint>


constexpr int factorial(int n){
    return (n == 1 || n == 0) ? 1 : factorial (n-1) * n;
}

// 4-term Taylor series — accurate only near 0
constexpr float mySin(float x){
    return x - (x*x*x)/factorial(3) + (x*x*x*x*x)/factorial(5)-(x*x*x*x*x*x*x)/factorial(7);
}

// std::fmod isn't constexpr-legal pre-C++23 (same issue as std::sin) —
// hand-written, standard-conforming replacement.
constexpr float constexprFmod(float x, float m) {
    while (x >= m) {
        x -= m;
    }
    while (x < 0.0f) {
        x += m;
    }
    return x;
}

constexpr float PI = 3.14159265358979323846f;

// Folds any x into [0, 2*PI), then uses quadrant symmetry to always call
// mySin() with an argument in [-PI/2, PI/2]
constexpr float sinReduced(float x) {
    float r = constexprFmod(x, 2.0f * PI);
    if (r < PI / 2.0f) {
        return mySin(r);
    }
    else if (r < PI) {
        return mySin(PI - r);
    }
    else if (r < 3.0f * PI / 2.0f) {
        return -mySin(r - PI);
    }
    else 
    {
        return -mySin(2.0f * PI - r);
    }
}

struct SinTable {
    float data[256];
};

constexpr SinTable buildSinLUT() {
    SinTable table{};
    for (uint32_t i = 0; i < 256; i++) {
        table.data[i] = sinReduced(0 + i * (2 * PI) / 256);
    }
    return table;
}

extern const SinTable g_sinTable = buildSinLUT();

int main() {


    return 0;
}