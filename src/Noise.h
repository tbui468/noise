//common header files
//utility functions

#ifndef NOISE_H
#define NOISE_H

#include "Vec.h"

float randf() {
    return static_cast<float>(rand()) / RAND_MAX;
}


float randf(float a, float b) {
    return (b - a) * randf() + a;
}

template<typename T>
Vec2<T> rand_unit_vector2() {
    float theta = randf(0.0f, 2.0f * static_cast<float>(M_PI));
    float x = cos(theta);
    float y = sin(theta); 
    return Vec2<T>(x, y);
}

template<typename T>
Vec3<T> rand_unit_vector3() {
    float z = randf(-1.0f, 1.0f);
    float theta = randf(0.0f, 2 * static_cast<float>(M_PI));
    float r = sqrt(1 - z * z);
    float x = r * cos(theta);
    float y = r * sin(theta); 
    return Vec3<T>(x, y, z);
}

//returns int in range [a, b)
int randi(int a, int b) {
    return rand() % (b - a) + a;
}

//linear interpolate
template<typename T = float>
float lerp(const T& a, const T& b, const T& t) {
    return a * (T(1) - t) + b * t;
}

//smooth out t with smoothstep
float smoothstep(const float& t) {
    return t * t * (3.0f - 2.0f * t);
}

//smooth out t with quintic function
float quintic(const float& t) {
    return t * t * t * (6.0f * t * t - 15.0f * t + 10.0f);
}

float quintic_deriv(const float& t) {
    return t * t * (30.0f * t * t - 60.0f * t + 30.0f);
}

//smooth out t with cosine
float cosine(const float& t) {
    return 0.5f * (1.0f - cos(t * float(M_PI)));
}

class Noise {
public:
    Noise() {};
    virtual float eval(const Vec2f& p) const = 0;
public:
    static const unsigned TABLESIZE = 256;
    unsigned m_permutationTable[TABLESIZE * 2]; //permutation table
};

class Noise3D {
public:
    Noise3D() {};
    virtual float eval(const Vec3f& p, Vec3f& deriv) const = 0;
public:
    static const unsigned TABLESIZE = 256;
    unsigned m_permutationTable[TABLESIZE * 2]; //permutation table
};

#endif //NOISE_H