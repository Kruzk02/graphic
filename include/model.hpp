#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "material.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include <string>

struct SubMesh
{
    Mesh mesh;
    Material material;
};

class Model
{
  public:
    Transform transform;
    std::string directory;

    auto loadFromFile(const std::string &path) -> bool;

    void addSubMesh(SubMesh &&subMesh)
    {
        subMeshes.push_back(std::move(subMesh));
    }

    void draw(const Shader &shader) const
    {
        for (const auto &[mesh, material] : subMeshes)
        {
            material.bind(shader);
            mesh.draw();
        }
    }

  private:
    std::vector<SubMesh> subMeshes;

    void processNode(aiNode *node, const aiScene *scene);
    auto processMesh(aiMesh *mesh, const aiScene *scene) -> SubMesh;
};
