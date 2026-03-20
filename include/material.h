#pragma once

#include "shader.h"
#include "texture.h"
#include <vector>

class Material {
public:
  void bind(const Shader &shader) const {
    for (GLint i = 0; i < textures.size(); ++i) {
      textures[i].bind(i);
      shader.setInt(toString(textures[i].getType()), i);
    }
  }

  void addTexture(Texture &&texture) { textures.push_back(std::move(texture)); }

private:
  std::vector<Texture> textures;
};
