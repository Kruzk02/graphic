#pragma once
#include "material.h"
#include "mesh.h"
#include "transform.h"

struct SubMesh {
    Mesh mesh;
    Material material;
};

class Model {
public:
    Transform transform;

    void addSubMesh(SubMesh&& subMesh) {
        subMeshes.push_back(std::move(subMesh));
    }

    void draw(const Shader& shader) const {
        for (const auto&[mesh, material] : subMeshes) {
            material.bind(shader);
            mesh.draw();
        }
    }
private:
    std::vector<SubMesh> subMeshes;
};