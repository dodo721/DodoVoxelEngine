#ifndef VOXOBJ_HPP
#define VOXOBJ_HPP

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "mesh.hpp"

namespace vox {
    namespace obj {

        class VoxObj {
            public:
                Mesh mesh;
                std::string name;
                std::string texpath;
                glm::vec3 position = glm::vec3(0);
                VoxObj();
                VoxObj(std::string name, Mesh mesh, std::string texpath);
                VoxObj(std::string name, Mesh mesh, std::string texpath, glm::vec3 position);
        };

    }
}

#endif