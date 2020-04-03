#include <stdlib.h>
#include <stdio.h>

#include "obj.hpp"
#include "macros.hpp"
#include "array_utils.hpp"

using namespace glm;
using namespace std;
using namespace obj3D;

Obj::Obj () {}

Obj::Obj (int vertLength, float* verts, float* uv): vertSize(vertLength) {
    vertices = arrayToVector<float>(verts, vertLength * 3);
    uvs = arrayToVector<float>(uv, vertLength * 2);
}

Obj::Obj (vector<float> verts, vector<float> uv): vertices(verts), uvs(uv) {}

int Obj::getVertSize () {
    return vertices.size();
}

int Obj::getUVSize () {
    return uvs.size();
}

int Obj::getIndicesSize () {
    return indices.size();
}

float* Obj::getVerts () {
    return &vertices[0];
}

float* Obj::getUVs () {
    return &uvs[0];
}

unsigned int* Obj::getIndices () {
    return &indices[0];
}