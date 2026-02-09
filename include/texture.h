#pragma once

#include <string>

#include "glad/glad.h"

class Texture {
public:
    explicit Texture(const std::string &path, bool flip = false);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture&& other) noexcept {
        id = other.id;
        other.id = 0;
    }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &id);
            id = other.id;
            other.id = 0;
        }
        return *this;
    }

    void bind(GLuint unit = 0) const;
    [[nodiscard]] GLuint getId() const { return id; }
private:
    GLuint id = 0;
};
