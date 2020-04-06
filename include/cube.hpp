#ifndef CUBE_HPP
#define CUBE_HPP

#include "obj.hpp"

extern float* cube_vertex_data;
extern float* cube_colour_data;
obj3D::Obj* createCube (glm::vec3 position);
obj3D::Obj* createCube ();

#endif