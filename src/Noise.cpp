#define _USE_MATH_DEFINES

#include <cmath>
//#include <cstdio>
//#include <random>
//#include <functional>
#include <iostream>
#include <fstream>
#include "Vec.h"
#include "Generator.h"
#include "PolyMesh.h"

//int main(int argc, char **argv)
int main()
{
    //create polygon mesh .obj file
    PerlinNoise3D noise;
    uint32_t subdWidth = 30;
    uint32_t subdHeight = 30;
    PolyMesh* poly = createPolyMesh(3, 3, subdWidth, subdHeight);
    
    //fractal polygon mesh
    /*
    float maxAmp = 0.0f;
    for(uint32_t i = 0; i < poly->vertexCount; ++i) {
        float freq = 0.5f;
        float amp = 1.0f;
        float fractal = 0.0f;
        Vec3f deriv;
        for (unsigned layer = 0; layer < 5; ++layer)
        {
            Vec3f p((poly->vertices[i].x + 0.0f), 0.0f, (poly->vertices[i].z + 0.0f));
            //normalize to range[0, 1], then multiply by amp
            fractal += amp * 0.5f * (noise.eval(p * freq, deriv) + 1.0f); //can you sum derivatives????
            freq *= 2.0f;
            amp *= 0.5f;
        }
        if(fractal > maxAmp) maxAmp = fractal;
        poly->vertices[i].y = fractal;
    }

    for(uint32_t i = 0; i < poly->vertexCount; ++i) {
        poly->vertices[i].y /= maxAmp;
    }*/

    //value noise polygon mesh
    for (uint32_t i = 0; i < poly->vertexCount; ++i)
    {
        Vec3f deriv;
        Vec3f p((poly->vertices[i].x), 0.0f, (poly->vertices[i].z));
        poly->vertices[i].y = 0.5f * (noise.eval(p * 1.0f, deriv) + 1.0f);
        poly->normals[i] = Vec3f(-deriv.x, 1.0f, -deriv.z).unit();
    }

   /* 
    //set surface normal using forward difference
    //doesn't work for last row/column of vertices
    for(uint32_t i = 0; i < poly->faceCount; ++i) {
        uint32_t offset = i * 4;
        Vec3f origin = poly->vertices[poly->vertexArray[offset]];
        Vec3f xv = poly->vertices[poly->vertexArray[offset + 1]];
        Vec3f zv = poly->vertices[poly->vertexArray[offset + 3]];
        Vec3f xTangent(xv.x - origin.x, xv.y - origin.y, xv.z - origin.z);
        Vec3f zTangent(zv.x - origin.x, zv.y - origin.y, zv.z - origin.z);
        poly->normals[poly->vertexArray[offset]] = cross3(zTangent, xTangent).unit();
    }*/

    poly->exportToObj();
    delete poly;

    //create 2d ppm file
    unsigned imageWidth = 512;
    unsigned imageHeight = 512;
    float *noiseMap = new float[imageWidth * imageHeight]{0};

//make noise type a paramter (eg ValueNoise or PerlinNoise object)
#if 0
    white_noise(noiseMap, imageWidth, imageHeight);
#elif 0
    value_noise(noiseMap, imageWidth, imageHeight, 0.02f);
#elif 0
    fractal3D(noiseMap, imageWidth, imageHeight, 5, 0.02f, 1.8f, 1.0f, 0.35f);
#elif 1
    turbulence3D(noiseMap, imageWidth, imageHeight, 5, 0.02f, 1.8f, 1.0f, 0.35f);
#elif 0
    marble3D(noiseMap, imageWidth, imageHeight, 5, 0.01f, 1.8f, 1.0f, 0.35f);
#else 1
    wood(noiseMap, imageWidth, imageHeight, 5, 0.02f, 1.8f, 1.0f, 0.35f);
#endif

    // output noise map to PPM
    std::ofstream ofs;
    ofs.open("./noise.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n"
        << imageWidth << " " << imageHeight << "\n255\n";
    for (unsigned k = 0; k < imageWidth * imageHeight; ++k)
    {
        unsigned char n = static_cast<unsigned char>(noiseMap[k] * 255);
        ofs << n << n << n;
    }
    ofs.close();

    delete[] noiseMap;

    return 0;
}