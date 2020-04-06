#ifndef MESH_HPP
#define MESH_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;

namespace vox {
    namespace obj {

        class Mesh {
            vector<float> vertices;
            vector<float> uvs;
            vector<unsigned int> indices;
            public:
                unsigned int VAO;
                unsigned int vertexBuffer;
                unsigned int uvBuffer;
                unsigned int indiceBuffer;
                unsigned int vertSize;
                unsigned int uvSize;
                unsigned int texid;
                Mesh();
                Mesh(int tris, float* verts, float* uv);
                Mesh(vector<float> verts, vector<float> uv);
                int getVertSize();
                int getUVSize();
                int getIndicesSize();
                float* getVerts();
                float* getUVs();
                unsigned int* getIndices();
        };

    }
}

#endif