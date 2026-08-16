 template <typename T> constexpr T add(T a, T b) {
    return a + b; 
}

int smoke() { 
    return add(2, 3); 
}   // forces instantiation so there's code to inspect