#ifndef VOXMESH_HPP
#define VOXMESH_HPP

#include "mesh.hpp"

namespace vox::obj {

    class VoxMesh : public Mesh {
        bool*** data;
        int width = 0;
        int height = 0;
        int length = 0;
        void calcMesh ();
        public:
            bool extendable = false;
            VoxMesh();
            VoxMesh(bool*** data, int width, int height, int length);
            ~VoxMesh();
            bool appendUnit (int x, int y, int z);
    };

}

#endif