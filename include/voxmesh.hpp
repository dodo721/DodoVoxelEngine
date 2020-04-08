#ifndef VOXMESH_HPP
#define VOXMESH_HPP

namespace vox::obj {

    //class Mesh;

    class VoxMesh {
        bool*** data;
        int width = 0;
        int height = 0;
        int length = 0;
        //Mesh mesh;
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