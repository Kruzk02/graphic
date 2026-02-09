#pragma once

#include <vector>
#include "mesh.h"
#include "texture.h"

struct Material {
    std::vector<Texture> diffuse;
    std::vector<Texture> normal;
    std::vector<Texture> specular;
};

struct SubMesh {
    Mesh mesh;
    Material material;
};