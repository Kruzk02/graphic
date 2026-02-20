#pragma once

#include <vector>
#include "shader.h"
#include "texture.h"

struct MaterialTexture {
    Texture texture;
    std::string uniformName;
};

class Material {
public:
    void bind(const Shader& shader) const {
        for (GLint i = 0; i < textures.size(); ++i) {
            textures[i].texture.bind(i);
            shader.setInt(textures[i].uniformName, i);
        }
    }

private:
    std::vector<MaterialTexture> textures;
};