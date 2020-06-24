#ifndef POLYMESH_H
#define POLYMESH_H

#include "Noise.h"
#include <fstream>

class PolyMesh {
public:
    PolyMesh() : vertices(nullptr), st(nullptr), normals(nullptr) {};
    ~PolyMesh() {
        if(vertices) delete[] vertices;
        if(st) delete[] st;
        if(normals) delete[] normals;
    }
    void exportToObj();
public:
    Vec3f* vertices;
    Vec2f* st; //surface texture????
    Vec3f* normals;
    uint32_t* faceArray; 
    uint32_t* vertexArray;
    uint32_t vertexCount;
    uint32_t faceCount;
};

void PolyMesh::exportToObj() {
    std::ofstream ofs;
    ofs.open("polyMesh.obj", std::ios_base::out); //output file

    //outputs vertex information in following format
    //v 123.4 123.4 123.4
    for(uint32_t i = 0; i < vertexCount; ++i) {
        ofs << "v " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z << '\n';
    }

    //outputs st information in following format (texture coordinates???)
    //vt 0.1 .34
    for(uint32_t i = 0; i < vertexCount; ++i) {
        ofs << "vt " << st[i].x << " " << st[i].y << '\n';
    }

    //outputs normals
    //vn 1.0 3.0 -0.3
    for(uint32_t i = 0; i < vertexCount; ++i) {
        ofs << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << '\n';
    }

    //outputs faces data (vertex index/texture index/normal index, with starting value equal to 1)
    //f 1/1/1 4/4/4 7/7/7 3/3/3
    for(uint32_t i = 0, offset = 0; i < faceCount; ++i) {
        ofs << "f ";
        for(uint32_t j = 0; j < faceArray[i]; ++j) {
            uint32_t objIndex = vertexArray[offset + j] + 1; //the + 1 is bc .obj files faces data starts at 1
            ofs << objIndex << '/' << objIndex << '/' << objIndex << ((j == (faceArray[i] - 1)) ? "" : " ");
        }
        ofs << '\n';
        offset += faceArray[i];
    }
    
    ofs.close();
}



//creates mesh centered around origin
//subdivisions are number of grid boxes (number of vertices are subdivision + 1)
PolyMesh* createPolyMesh(uint32_t width = 1, uint32_t height = 1, uint32_t subdivisionWidth = 40, uint32_t subdivisionHeight = 40) {
    //initialize PolyMesh object and member variables
    PolyMesh* poly = new PolyMesh;
    poly->vertexCount = (subdivisionWidth + 1) * (subdivisionHeight + 1);
    poly->vertices = new Vec3f[poly->vertexCount];
    poly->normals = new Vec3f[poly->vertexCount];
    poly->st = new Vec2f[poly->vertexCount];

    //set vertices and texture coordinates
    //we're going to toss these Vec3 into noise function eval(Vec3) to interpolate
    //negative Vec3 is okay since our eval(Vec3) function and noise is periodic
    float invSubdivisionWidth = 1.0f / subdivisionWidth;
    float invSubdivisionHeight  = 1.0f / subdivisionHeight;
    for(uint32_t row = 0; row <= subdivisionHeight; ++row) {
        for(uint32_t col = 0; col <= subdivisionWidth; ++col) {
            poly->vertices[row * (subdivisionWidth + 1) + col] = Vec3f(width * (col * invSubdivisionWidth - 0.5f), 0.0f, height * (row * invSubdivisionHeight - 0.5f));
            poly->st[row * (subdivisionWidth + 1) + col] = Vec2f(col * invSubdivisionWidth, row * invSubdivisionHeight); //set texture coordinates to range [0, 1]
        }
    }

    //set polygon faces (number of vertices in a single face)
    poly->faceCount = subdivisionWidth * subdivisionHeight;
    poly->faceArray = new uint32_t[poly->faceCount];
    for(uint32_t i = 0; i < poly->faceCount; ++i) {
        poly->faceArray[i] = 4; //we want quads as our polygons
    }

    //set vertexArray (clusters of 4 vertices)
    poly->vertexArray = new uint32_t[4 * poly->faceCount];
    for(uint32_t row = 0, offset = 0; row < subdivisionHeight; ++row) {
        for(uint32_t col = 0; col < subdivisionWidth; ++col) {
            poly->vertexArray[offset] = row * (subdivisionWidth + 1) + col; //start
            poly->vertexArray[offset + 1] = row * (subdivisionWidth + 1) + col + 1; //x + 1
            poly->vertexArray[offset + 2] = (row + 1) * (subdivisionWidth + 1) + col + 1; //x + 1, z + 1
            poly->vertexArray[offset + 3] = (row + 1) * (subdivisionWidth + 1) + col; //z + 1
            offset += 4;
        }
    }

    return poly;
}


#endif //POLYMESH