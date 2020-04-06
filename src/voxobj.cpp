#include <stdlib.h>
#include <stdio.h>

#include "voxobj.hpp"
#include "macros.hpp"
#include "array_utils.hpp"

using namespace glm;
using namespace std;
using namespace vox::obj;

VoxObj::VoxObj () {}

VoxObj::VoxObj (string name, Mesh mesh, string texpath): name(name), mesh(mesh), texpath(texpath) {}
VoxObj::VoxObj (string name, Mesh mesh, string texpath, vec3 position): name(name), mesh(mesh), texpath(texpath), position(position) {}