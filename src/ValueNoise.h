#ifndef VALUENOISE_H
#define VALUENOISE_H

#include "Noise.h"

class ValueNoise : public Noise{
public:
    ValueNoise() : Noise(){
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_value[i] = float(rand()) / RAND_MAX;
            m_permutationTable[i] = i;
        } 

        //create 2-d permutation table
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            unsigned k = randi(0, TABLESIZE);
            std::swap(m_permutationTable[i], m_permutationTable[k]);
            m_permutationTable[i + TABLESIZE] = m_permutationTable[i];
        }
    }

    //given a pair of coordinates (i, j), return the interpolated value
    //using the precalculated values and the permutation table
    virtual float eval(const Vec2f& p) const { 
        //find the four surrounding lattice points rx0, rx1, ry0, and ry1
        //get those integer values
        int rx0 = int(floor(p.x));
        int rx1 = rx0 + 1;
        int ry0 = int(floor(p.y));
        int ry1 = ry0 + 1;

        //find tx, and ty
        float tx = (p.x - float(rx0)); 
        float ty = (p.y - float(ry0));

        //take modulus(taking negative into account) to make sure
        //integer value is in range[0, TABLESIZE]
        while(rx0 <= 0)
            rx0 += TABLESIZE;
        rx0 %= TABLESIZE;

        while(rx1 <= 0)
            rx1 += TABLESIZE;
        rx1 %= TABLESIZE;

        while(ry0 <= 0)
            ry0 += TABLESIZE;
        ry0 %= TABLESIZE;

        while(ry1 <= 0)
            ry1 += TABLESIZE;
        ry1 %= TABLESIZE;

        //use the four lattice points to find the random values in array m_value
        //look up values in permutation table twice
        //essentially, it makes each row/column a permutation of all random floats in m_value
        float c00 = m_value[m_permutationTable[m_permutationTable[rx0] + ry0]];
        float c10 = m_value[m_permutationTable[m_permutationTable[rx1] + ry0]];
        float c01 = m_value[m_permutationTable[m_permutationTable[rx0] + ry1]];
        float c11 = m_value[m_permutationTable[m_permutationTable[rx1] + ry1]];

        
        //smooth x and interpolate between c00 and c10
        //and between c01 and c11
        float sx = smoothstep(tx);
        float xi0 = lerp(c00, c10, sx);
        float xi1 = lerp(c01, c11, sx);

        //smooth y and interpolate between xi0 and xi1
        float sy = smoothstep(ty);

        return lerp(xi0, xi1, sy);
    }

public:
    float m_value[TABLESIZE]; //random values
};

#endif //VALUENOISE_H