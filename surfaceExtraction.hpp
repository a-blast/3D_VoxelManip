#include <cstdint>
#include <iostream>
#include "VoxelModel.hpp"

#ifndef surfaceExtraction_h
#define surfaceExtraction_h

enum FaceType {
    NX,
    NY,
    NZ,
    PX,
    PY,
    PZ
};

struct Triangle{
    float normal[3];
    float v1[3];
    float v2[3];
    float v3[3];
};

void extractFace(int x, int y, int z, FaceType face, Triangle& t1, Triangle& t2);

void writeSTL(VoxelModel model, const char* filename);

void writeTriangles(FaceType face, FILE* stl, Triangle* newTri, int x, int y, int z);

#endif
