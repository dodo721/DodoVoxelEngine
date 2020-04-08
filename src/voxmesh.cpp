#include "voxmesh.hpp"

using namespace vox::obj;

VoxMesh::VoxMesh () {}

VoxMesh::VoxMesh (bool*** data, int width, int height, int length): data(data), width(width), height(height), length(length) {
    calcMesh();
}

VoxMesh::~VoxMesh () {
    for (int i  = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            delete[] data[i][j];
        }
        delete[] data[i];
    }
    delete[] data;
}

void VoxMesh::calcMesh () {
    
    

}

bool VoxMesh::appendUnit (int x, int y, int z) {

    if (x < width && y < height && z < length) {

        if (data[x][y][z]) // Unit already exists, fail
            return false;
        
        data[x][y][z] = true;

    } else if (extendable) { // Unit is out of bounds, but bounds can be extended

    } else return false; // Unit is out of constrained bounds, fail

}