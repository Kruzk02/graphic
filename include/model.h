#pragma once
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "material.h"
#include "mesh.h"
#include "transform.h"
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

    bool loadFromFile(const std::string &path);

    void addSubMesh(SubMesh &&subMesh)
    {
        subMeshes.push_back(std::move(subMesh));
    }

    void draw(const Shader &shader) const
    {
        std::cout << "Drawing " << subMeshes.size() << " submeshes\n";
        for (const auto &[mesh, material] : subMeshes)
        {
            material.bind(shader);
            mesh.draw();
        }
    }

  private:
    std::vector<SubMesh> subMeshes;

    void processNode(aiNode *node, const aiScene *scene);
    SubMesh processMesh(aiMesh *mesh, const aiScene *scene);
};
