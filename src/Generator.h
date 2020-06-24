#ifndef GENERATOR_H
#define GENERATOR_H

#include "Vec.h"
#include "ValueNoise.h"
#include "PerlinNoise.h"
#include "PerlinNoise3D.h"
#include "ValueNoise3D.h"


void white_noise(float* buffer, unsigned width, unsigned height) {
    ValueNoise noise;
    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            Vec2f pos(static_cast<float>(i), static_cast<float>(j));
            buffer[i * width + j] = noise.eval(pos); //frequency if 1
        }
    }
}

void value_noise(float* buffer, unsigned width, unsigned height, float frequency) {
    PerlinNoise noise;
    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            Vec2f pos(static_cast<float>(i), static_cast<float>(j));
            buffer[i * width + j] = noise.eval(pos * frequency); 
        }
    }
}


void fractal(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    ValueNoise noise;
    float maxAmplitude = 0.0f;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            for(unsigned layer = 0; layer < layers; ++layer) {
                buffer[i * width + j] += currentAmp * noise.eval(Vec2f(static_cast<float>(i), static_cast<float>(j)) * currentFreq);
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}


void fractal3D(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    PerlinNoise3D noise;
    float maxAmplitude = 0.0f;
    Vec3f deriv;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            for(unsigned layer = 0; layer < layers; ++layer) {
                buffer[i * width + j] += currentAmp * noise.eval(Vec3f(static_cast<float>(i), static_cast<float>(j), 0.0f) * currentFreq, deriv);
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}



void turbulence(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    PerlinNoise noise;
    float maxAmplitude = 0.0f;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            for(unsigned layer = 0; layer < layers; ++layer) {
                buffer[i * width + j] += abs(currentAmp * (2.0f * noise.eval(Vec2f(static_cast<float>(i), static_cast<float>(j)) * currentFreq) - 1.0f));
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}


void turbulence3D(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    PerlinNoise3D noise;
    float maxAmplitude = 0.0f;
    Vec3f deriv;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            for(unsigned layer = 0; layer < layers; ++layer) {
                buffer[i * width + j] += abs(currentAmp * (2.0f * noise.eval(Vec3f(static_cast<float>(i), static_cast<float>(j), 0.0f) * currentFreq, deriv) - 1.0f));
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}


void marble(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    PerlinNoise noise;
    float maxAmplitude = 0.0f;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            float marbleNoise = 0;
            for(unsigned layer = 0; layer < layers; ++layer) {
                marbleNoise += currentAmp * noise.eval(Vec2f(static_cast<float>(i), static_cast<float>(j)) * currentFreq);
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            //modulate with sine wave with i or j (changes pattern orientation)
            //scale j/i to make stripes thicker/thinner. 
            //scale marbleNoise to make pattern closer to sinusoid, or more chaotic
            buffer[i * width + j] = sin(static_cast<float>(j) * .05f + marbleNoise * 6.0f) + 1.0f; 
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}

void marble3D(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    PerlinNoise3D noise;
    float maxAmplitude = 0.0f;
    Vec3f deriv;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            float marbleNoise = 0;
            for(unsigned layer = 0; layer < layers; ++layer) {
                marbleNoise += currentAmp * noise.eval(Vec3f(static_cast<float>(i), static_cast<float>(j), 0.0f) * currentFreq, deriv);
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            //modulate with sine wave with i or j (changes pattern orientation)
            //scale j/i to make stripes thicker/thinner. 
            //scale marbleNoise to make pattern closer to sinusoid, or more chaotic
            buffer[i * width + j] = sin(static_cast<float>(j) * .05f + marbleNoise * 6.0f) + 1.0f; 
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}


void wood(float* buffer, unsigned width, unsigned height, unsigned layers, float frequency, float freqScalar, float amplitude, float ampScalar) {
    ValueNoise noise;
    float maxAmplitude = 0.0f;

    for(unsigned i = 0; i < height; ++i) {
        for(unsigned j = 0; j < width; ++j) {
            float currentFreq = frequency;
            float currentAmp = amplitude;
            float woodNoise = 0.0f;
            for(unsigned layer = 0; layer < layers; ++layer) {
                woodNoise += currentAmp * noise.eval(Vec2f(static_cast<float>(i), static_cast<float>(j)) * currentFreq);
                currentFreq *= freqScalar;
                currentAmp *= ampScalar;
            }
            woodNoise *= 10;
            buffer[i * width + j] = woodNoise - int(woodNoise);
            if (buffer[i * width + j] > maxAmplitude)
                maxAmplitude = buffer[i * width + j];
        }
    }
    for (unsigned k = 0; k < width * height; ++k)
    {
        buffer[k] /= maxAmplitude;
    }
}



#endif //GENERATOR_H