
#include <cstdio>

struct Celsius {};
struct Pascal {};

template <typename Unit>
struct SensorReading {
    float value;
    explicit SensorReading(float v) : value(v) {}

    SensorReading operator+(const SensorReading& other) const {
        return SensorReading(value + other.value);
    }
};

int main() {
    SensorReading<Celsius> t1(20.0f);
    SensorReading<Celsius> t2(5.0f);
    SensorReading<Celsius> t3 = t1 + t2;
    // SensorReading<Pascal> p1(1000.0f);
    // SensorReading<Celsius> bad1 = 20.0f;
    // SensorReading<Celsius> bad2 = t1 + p1;
    printf("t1 + t2 = %.2f Celsius\n", t3.value);

    SensorReading<Pascal> p1(101325.0f);

    // Verified compile errors (see week35 notes):
    // (1) SensorReading<Celsius> bad1 = 20.0f;
    //     -> error: conversion from 'float' to non-scalar type
    //        'SensorReading<Celsius>' requested
    //     (explicit constructor blocks implicit float construction)
    //
    // (2) SensorReading<Celsius> bad2 = t1 + p1;
    //     -> error: no match for 'operator+' (operand types are
    //        'SensorReading<Celsius>' and 'SensorReading<Pascal>')
    //     (injected class name scopes operator+ to same-Unit only)

    return 0;
}