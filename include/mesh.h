#pragma once
#include <vector>

#include "glad/glad.h"
#include "glm/ext/vector_float3.hpp"
#include "layout.h"
#include <glm/gtc/matrix_transform.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec3 BiTangent;
    int mBoneIds[MAX_BONE_INFLUENCE];
    float mWeights[MAX_BONE_INFLUENCE];

    static VertexLayout getLayout()
    {
        return {{
                    {0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position)},
                    {1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal)},
                    {2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord)},
                },
                sizeof(Vertex)};
    }
};

class Mesh
{
  public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
         const VertexLayout &layout)
        : indexCount(indices.size())
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size()) *
                         static_cast<GLsizeiptr>(sizeof(Vertex)),
                     vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(indices.size()) *
                         static_cast<GLsizeiptr>(sizeof(unsigned)),
                     indices.data(), GL_STATIC_DRAW);

        for (const auto &[index, count, type, normalized, offset] : layout.attributes)
        {
            glEnableVertexAttribArray(index);
            if (type == GL_INT || type == GL_UNSIGNED_INT)
            {
                glVertexAttribIPointer(index, count, type, layout.stride,
                                       reinterpret_cast<void *>(offset));
            }
            else
            {
                glVertexAttribPointer(index, count, type, normalized, layout.stride,
                                      reinterpret_cast<void *>(offset));
            }
        }

        glBindVertexArray(0);
    }

    void draw(const GLenum mode = GL_TRIANGLES) const
    {
        glBindVertexArray(VAO);
        glDrawElements(mode, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    ~Mesh()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    Mesh(const Mesh &) = delete;

    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&other) noexcept
    {
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        indexCount = other.indexCount;
        other.VAO = other.VBO = other.EBO = 0;
    }

    Mesh &operator=(Mesh &&other) noexcept
    {
        if (this != &other)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);

            VAO = other.VAO;
            VBO = other.VBO;
            EBO = other.EBO;
            indexCount = other.indexCount;
            other.VAO = other.VBO = other.EBO = 0;
        }
        return *this;
    }

  private:
    GLuint VAO = 0, VBO = 0, EBO = 0;
    size_t indexCount = 0;
};
