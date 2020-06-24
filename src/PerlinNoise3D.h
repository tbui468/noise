
#ifndef PERLINNOISE3D_H
#define PERLINNOISE3D_H

#include <assert.h>
#include <iostream>
#include "Noise.h"




//3-d Perlin noise
class PerlinNoise3D : public Noise3D{
public:
    PerlinNoise3D() {
        //fill with random 2-d unit vectors
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_gradient[i] = rand_unit_vector3<float>();
            m_permutationTable[i] = i;
        }

        //create perumation table
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            int w = randi(0, TABLESIZE);
            std::swap(m_permutationTable[i], m_permutationTable[w]);
        }
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_permutationTable[i + TABLESIZE] = m_permutationTable[i];
        }
    }

    virtual float eval(const Vec3f& p, Vec3f& deriv) const {
        //find lowest x, y, z corner of cube and x + 1, y + 1, z + 1
        //to get all 8 corners of cube with coordinates
        //i0, i1, j0, j1, k0, k1
        //find parameters tx, ty, and tz
        int i0 = static_cast<int>(std::floor(p.x));
        int i1 = i0 + 1;
        int j0 = static_cast<int>(std::floor(p.y));
        int j1 = j0 + 1;
        int k0 = static_cast<int>(std::floor(p.z));
        int k1 = k0 + 1;

        //find parameter t
        float tx = p.x - std::floor(p.x);
        float ty = p.y - std::floor(p.y);
        float tz = p.z - std::floor(p.z);

        //mod to keep values in range [0, 255], also taking negative numbers into account
        i0 = i0 & 255;
        i1 = i1 & 255;
        j0 = j0 & 255;
        j1 = j1 & 255;
        k0 = k0 & 255;
        k1 = k1 & 255;

        //gradient vector at each corner of cube
        const Vec3f& g000 = m_gradient[hash(i0, j0, k0)];
        const Vec3f& g001 = m_gradient[hash(i0, j0, k1)];
        const Vec3f& g010 = m_gradient[hash(i0, j1, k0)];
        const Vec3f& g011 = m_gradient[hash(i0, j1, k1)];
        const Vec3f& g100 = m_gradient[hash(i1, j0, k0)];
        const Vec3f& g101 = m_gradient[hash(i1, j0, k1)];
        const Vec3f& g110 = m_gradient[hash(i1, j1, k0)];
        const Vec3f& g111 = m_gradient[hash(i1, j1, k1)];

        //vector from each corner to point P, normalized
        Vec3f p000 = Vec3f(tx, ty, tz).unit();
        Vec3f p001 = Vec3f(tx, ty, tz - 1.0f).unit();
        Vec3f p010 = Vec3f(tx, ty - 1.0f, tz).unit();
        Vec3f p011 = Vec3f(tx, ty - 1.0f, tz - 1.0f).unit();
        Vec3f p100 = Vec3f(tx - 1.0f, ty, tz).unit();
        Vec3f p101 = Vec3f(tx - 1.0f, ty, tz - 1.0f).unit();
        Vec3f p110 = Vec3f(tx - 1.0f, ty - 1.0f, tz).unit();
        Vec3f p111 = Vec3f(tx - 1.0f, ty - 1.0f, tz - 1.0f).unit();

        //smooth tx, ty and tz
        float u = quintic(tx); 
        float du = quintic_deriv(tx);
        float v = quintic(ty); 
        float dv = quintic_deriv(ty);
        float w = quintic(tz); 
        float dw = quintic_deriv(tz);

        float a = dot3(g000, p000);
        float b = dot3(g001, p001);
        float c = dot3(g010, p010);
        float d = dot3(g011, p011);
        float e = dot3(g100, p100);
        float f = dot3(g101, p101);
        float g = dot3(g110, p110);
        float h = dot3(g111, p111);

        float c0 = -a + e;
        float c1 = -a + c;
        float c2 = -a + b;
        float c3 = a - c - e + g;
        float c4 = a - b - e + f;
        float c5 = a - b - c + d;
        float c6 = -a + b + c - d + e - f - g + h;
        
        deriv.x = du * (c0 + c3 * v + c4 * w + c6 * v * w);
        deriv.y = dv * (c1 + c3 * u + c5 * w + c6 * u * w);
        deriv.z = dw * (c2 + c4 * u + c5 * v + c6 * u * v);

        //interpolate across z axis (one line) 
        //result is in range [0, 1]
        return a + (c0 * u) + (c1 * v) + (c2 * w) + (c3 * u * v) + (c4 * u * w) + (c5 * v * w) + (c6 * u * v * w);
    }
public:
    Vec3f m_gradient[TABLESIZE];
private:
    int hash(int i, int j, int k) const {
        return m_permutationTable[m_permutationTable[m_permutationTable[i] + j] + k];
    }
};

#endif //PERLINNOISE3D_H