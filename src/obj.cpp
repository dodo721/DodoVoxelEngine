#include <stdlib.h>
#include <stdio.h>

#include "obj.hpp"
#include "macros.hpp"
#include "array_utils.hpp"

using namespace glm;
using namespace std;
using namespace obj3D;

Obj::Obj () {}

Obj::Obj (int tris, float* verts, float* uv): vertSize(tris) {
    vertices = arrayToVector<float>(verts, tris * 3);
    uvs = arrayToVector<float>(uv, tris * 2);
    position = vec3(0);
}

Obj::Obj (vector<float> verts, vector<float> uv): vertices(verts), uvs(uv) {
    position = vec3(0);
}

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