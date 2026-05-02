#pragma once

#include "shader.hpp"
#include "texture.hpp"
#include <string>
#include <vector>

class Material
{
  public:
    void bind(const Shader &shader) const
    {
        GLint unit = 0;

        for (const auto &tex : textures)
        {
            tex.bind(unit);

            std::string uniformName = toString(tex.getType()) + std::to_string(unit);

            shader.setInt(uniformName, unit);
            unit++;
        }
    }

    void addTexture(Texture &&texture)
    {
        textures.push_back(std::move(texture));
    }

  private:
    std::vector<Texture> textures;
};
