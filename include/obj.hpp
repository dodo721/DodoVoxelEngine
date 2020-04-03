#ifndef OBJ_HPP
#define OBJ_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;

namespace obj3D {

    class Obj {
        vector<float> vertices;
        vector<float> uvs;
        vector<unsigned int> indices;
        public:
            unsigned int VAO;
            unsigned int vertexBuffer;
            unsigned int uvBuffer;
            unsigned int indiceBuffer;
            unsigned int texid;
            unsigned int vertSize;
            unsigned int uvSize;
            std::string name;
            std::string texpath;
            glm::vec3 position;
            glm::vec3 rotation;
            Obj();
            Obj(int vertLength, float* verts, float* uv);
            Obj(vector<float> verts, vector<float> uv);
            int getVertSize();
            int getUVSize();
            int getIndicesSize();
            float* getVerts();
            float* getUVs();
            unsigned int* getIndices();
    };

}

#endif