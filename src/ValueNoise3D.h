
#ifndef VALUENOISE3D_H
#define VALUENOISE3D_H

#include "Noise.h"

class ValueNoise3D : public Noise3D{
public:
    ValueNoise3D() : Noise3D(){
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_value[i] = float(rand()) / RAND_MAX;
            m_permutationTable[i] = i;
        } 

        //create 2-d permutation table
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            unsigned k = randi(0, TABLESIZE);
            std::swap(m_permutationTable[i], m_permutationTable[k]);
        }

        //fill in second half of table
        for(unsigned i = 0; i < TABLESIZE; ++i) {
            m_permutationTable[i + TABLESIZE] = m_permutationTable[i];
        }
    }

    //given a pair of coordinates (i, j), return the interpolated value
    //using the precalculated values and the permutation table
    virtual float eval(const Vec3f& p, Vec3f& deriv) const { 
        //get those integer values of 8 corners of cube
        int i0 = static_cast<int>(floor(p.x));
        int i1 = i0 + 1;
        int j0 = static_cast<int>(floor(p.y));
        int j1 = j0 + 1;
        int k0 = static_cast<int>(floor(p.z));
        int k1 = k0 + 1;

        //find tx, and ty
        float tx = (p.x - float(i0)); 
        float ty = (p.y - float(j0));
        float tz = (p.z - float(k0));

        //smoothstep t parameters
        float sx = smoothstep(tx);
        float sy = smoothstep(ty);
        float sz = smoothstep(tz);

        //take modulus(taking negative into account) to make sure
        //integer value is in range[0, TABLESIZE]
        i0 = i0 & 255;
        i1 = i1 & 255;
        j0 = j0 & 255;
        j1 = j1 & 255;
        k0 = k0 & 255;
        k1 = k1 & 255;


        //use the four lattice points to find the random values in array m_value
        //look up values in permutation table twice
        //essentially, it makes each row/column a permutation of all random floats in m_value
        float c000 = m_value[hash(i0, j0, k0)];
        float c001 = m_value[hash(i0, j0, k1)];
        float c010 = m_value[hash(i0, j1, k0)];
        float c011 = m_value[hash(i0, j1, k1)];
        float c100 = m_value[hash(i1, j0, k0)];
        float c101 = m_value[hash(i1, j0, k1)];
        float c110 = m_value[hash(i1, j1, k0)];
        float c111 = m_value[hash(i1, j1, k1)];


        //interp along x - axis
        float x00 = lerp(c000, c100, sx);
        float x01 = lerp(c001, c101, sx);
        float x10 = lerp(c010, c110, sx);
        float x11 = lerp(c011, c111, sx);

        //interp along y - axis
        float y0 = lerp(x00, x10, sy);
        float y1 = lerp(x01, x11, sy);

        //interp along z - axis
        return lerp(y0, y1, sz);
    }

public:
    float m_value[TABLESIZE]; //random values
private:
    int hash(int i, int j, int k) const {
        return m_permutationTable[m_permutationTable[m_permutationTable[i] + j] + k];
    }
};

#endif //VALUENOISE3D_H