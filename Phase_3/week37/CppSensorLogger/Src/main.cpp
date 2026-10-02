// Src/main.cpp
#include "fixed_format.hpp"

volatile std::size_t g_sink;   // volatile store = observable, can't be optimized away

int main()
{
    char buf[16];
    g_sink = format_fixed(buf, sizeof buf, -150, 2);
    for (;;) {}
}