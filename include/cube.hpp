#ifndef CUBE_HPP
#define CUBE_HPP

#include "voxobj.hpp"

extern float* cube_vertex_data;
extern float* cube_colour_data;
vox::obj::VoxObj createCube (glm::vec3 position);
vox::obj::VoxObj createCube ();

#endif