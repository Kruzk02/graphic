#pragma once

#include <string>

#include "glad/glad.h"

enum class TextureType {
    Diffuse,
    Normal,
    Specular,
    Roughness,
    Metallic,
    AO
};

inline std::string toString(const TextureType type) {
    switch (type) {
        case TextureType::Diffuse:    return "Diffuse";
        case TextureType::Normal: return "Normal";
        case TextureType::Specular:   return "Specular";
        case TextureType::Roughness:   return "Roughness";
        case TextureType::Metallic:   return "Metallic";
        case TextureType::AO:   return "AO";
        default:            return "Unknown";
    }
}

class Texture {
public:
    explicit Texture(const std::string &path, TextureType type, bool flip = false);
    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    Texture(Texture&& other) noexcept : id(other.id), type(other.type) {
        other.id = 0;
    }

    Texture& operator=(Texture&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &id);
            id = other.id;
            type = other.type;
            other.id = 0;
        }
        return *this;
    }

    void bind(GLuint unit = 0) const;
    [[nodiscard]] GLuint getId() const { return id; }
    [[nodiscard]] TextureType getType() const { return type; }
private:
    GLuint id = 0;
    TextureType type;
};
