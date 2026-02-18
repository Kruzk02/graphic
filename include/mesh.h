#pragma once
#include <vector>

#include "glad/glad.h"
#include "layout.h"
#include "texture.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec2 BiTangent;
    int mBoneIds[MAX_BONE_INFLUENCE];
    float mWeights[MAX_BONE_INFLUENCE];
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         const VertexLayout &layout)
        : indexCount(indices.size()),
          vertices(std::move(vertices)),
          indices(std::move(indices)) {

        setupMesh(layout);
    }

    void draw(const GLenum mode = GL_TRIANGLES) const {
        glBindVertexArray(VAO);
        glDrawElements(mode, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    Mesh(const Mesh &) = delete;

    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&other) noexcept {
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;
        indexCount = other.indexCount;
        other.VAO = other.VBO = other.EBO = 0;
    }

    Mesh &operator=(Mesh &&other) noexcept {
        if (this != &other) {
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
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    void setupMesh(const VertexLayout &layout) {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(vertices.size()) * static_cast<GLsizeiptr>(sizeof(float)),
                     vertices.data(),
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     static_cast<GLsizeiptr>(indices.size()) * static_cast<GLsizeiptr>(sizeof(unsigned)),
                     indices.data(),
                     GL_STATIC_DRAW);

        for (const auto &[index, count, type, normalized, offset]: layout.attributes) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                count,
                type,
                normalized,
                layout.stride,
                reinterpret_cast<void *>(offset)
            );
        }

        glBindVertexArray(0);
    }
};
