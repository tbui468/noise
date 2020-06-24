#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <assert.h>
#include <iostream>
#include "Noise.h"


//2-d Perlin noise
class PerlinNoise : public Noise{
public:
    PerlinNoise() {
        //fill with random 2-d unit vectors
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_gradient[i] = rand_unit_vector2<float>();
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

    virtual float eval(const Vec2f& p) const {
        //find lowest x, y, z corner of cube and x + 1, y + 1, z + 1
        //to get all 8 corners of cube with coordinates
        //i0, i1, j0, j1, k0, k1
        //find parameters tx, ty, and tz
        int i0 = static_cast<int>(std::floor(p.x));
        int i1 = i0 + 1;
        int j0 = static_cast<int>(std::floor(p.y));
        int j1 = j0 + 1;

        //find parameter t
        float tx = p.x - std::floor(p.x);
        float ty = p.y - std::floor(p.y);


        i0 = i0 & 255;
        i1 = i1 & 255;
        j0 = j0 & 255;
        j1 = j1 & 255;

        assert(i0 >= 0 && i0 < 256);
        assert(i1 >= 0 && i1 < 256);
        assert(j0 >= 0 && j0 < 256);
        assert(j1 >= 0 && j1 < 256);


        //use permutation table and i/j/k coordinates to look up corner vectors
        //dot corner vector and (NORMALED!!!) vector from corner to point P
        const Vec2f& v00 = m_gradient[m_permutationTable[m_permutationTable[i0] + j0]];
        float c00 = dot2(v00, Vec2f(tx, ty).unit());

        const Vec2f& v01 = m_gradient[m_permutationTable[m_permutationTable[i0] + j1]];
        float c01 = dot2(v01, Vec2f(tx, ty - 1.0f).unit());

        const Vec2f& v10 = m_gradient[m_permutationTable[m_permutationTable[i1] + j0]];
        float c10 = dot2(v10, Vec2f(tx - 1.0f, ty).unit());

        const Vec2f& v11 = m_gradient[m_permutationTable[m_permutationTable[i1] + j1]];
        float c11 = dot2(v11, Vec2f(tx - 1.0f, ty - 1.0f).unit());


        assert(c00 <= 1.1f && c00 >= -1.1f);
        assert(c01 <= 1.1f && c01 >= -1.1f);
        assert(c10 <= 1.1f && c10 >= -1.1f);
        assert(c11 <= 1.1f && c11 >= -1.1f);


        //smooth tx, ty and tz
        float sx = smoothstep(tx); 
        float sy = smoothstep(ty); 

        //interpoalte across x axis (two lines)
        float x0 = lerp(c00, c10, sx);
        float x1 = lerp(c01, c11, sx);

        //interpolate across z axis (one line)
        //normalize to range [0,1]
        return 0.5f * (lerp(x0, x1, sy) + 1.0f);
    }
public:
    Vec2f m_gradient[TABLESIZE];
};

#endif //PERLINNOISE