#pragma once
#include <vector>

#include "glad/glad.h"

struct VertexAttribute {
    GLuint index;
    GLint count;
    GLenum type;
    GLboolean normalized;
    size_t offset;
};

struct VertexLayout {
    std::vector<VertexAttribute> attributes;
    GLint stride;

    void push(GLuint index, GLint count, GLenum type, GLboolean normalized) {
        attributes.push_back(VertexAttribute{
            index,
            count,
            type,
            normalized,
            static_cast<size_t>(stride)
            });
        stride += count * sizeof(float);
    }
};
