#include "model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/mesh.h"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "mesh.hpp"
#include <vector>

auto Model::loadFromFile(const std::string &path) -> bool
{
    Assimp::Importer importer;

    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_FlipUVs);

    if (!scene || !scene->mRootNode)
    {
        return false;
    }

    directory = path.substr(0, path.find_last_of("/\\"));
    processNode(scene->mRootNode, scene);
    return true;
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        subMeshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

auto Model::processMesh(aiMesh *mesh, const aiScene *scene) -> SubMesh
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex v;
        v.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        if (mesh->HasNormals())
        {
            v.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }

        if (mesh->mTextureCoords[0])
        {
            v.TexCoord = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }

        vertices.push_back(v);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    Mesh myMesh(vertices, indices, Vertex::getLayout());

    Material material;
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial *aiMat = scene->mMaterials[mesh->mMaterialIndex];

        auto loadTextures = [&](aiTextureType type, TextureType myType) -> void
        {
            for (unsigned int i = 0; i < aiMat->GetTextureCount(type); i++)
            {
                aiString str;
                aiMat->GetTexture(type, i, &str);

                std::string path = str.C_Str();

                material.addTexture(Texture(directory + "/" + path, myType, true));
            }
        };

        loadTextures(aiTextureType_DIFFUSE, TextureType::Diffuse);
        loadTextures(aiTextureType_NORMALS, TextureType::Normal);
        loadTextures(aiTextureType_SPECULAR, TextureType::Specular);

        loadTextures(aiTextureType_METALNESS, TextureType::Metallic);
        loadTextures(aiTextureType_DIFFUSE_ROUGHNESS, TextureType::Roughness);
        loadTextures(aiTextureType_AMBIENT_OCCLUSION, TextureType::AO);
    }

    return {.mesh = std::move(myMesh), .material = std::move(material)};
}
