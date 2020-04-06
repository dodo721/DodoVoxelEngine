#include <stdlib.h>
#include <stdio.h>

#include "mesh.hpp"
#include "macros.hpp"
#include "array_utils.hpp"

using namespace glm;
using namespace std;
using namespace vox::obj;

Mesh::Mesh () {}

Mesh::Mesh (int tris, float* verts, float* uv): vertSize(tris) {
    vertices = arrayToVector<float>(verts, tris * 3);
    uvs = arrayToVector<float>(uv, tris * 2);
}

Mesh::Mesh (vector<float> verts, vector<float> uv): vertices(verts), uvs(uv) {}

int Mesh::getVertSize () {
    return vertices.size();
}

int Mesh::getUVSize () {
    return uvs.size();
}

int Mesh::getIndicesSize () {
    return indices.size();
}

float* Mesh::getVerts () {
    return &vertices[0];
}

float* Mesh::getUVs () {
    return &uvs[0];
}

unsigned int* Mesh::getIndices () {
    return &indices[0];
}