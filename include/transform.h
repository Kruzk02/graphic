#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

/// @brief Represents a 3D transform (position, rotation, scale).
/// 
/// Provides utility functions to build a transformation matrix
/// and extract local direction vectors.
struct Transform {
    glm::vec3 position{0.0f}; // World space, units
    glm::vec3 rotation{0.0f}; // Euler angles in radians(local space)
    glm::vec3 scale{1.0f}; // local scale

    /// @brief Builds the transformation matrix.
    /// 
    /// The matrix is built in the following order:
    /// Translation -> Rotation (X, Y, Z) -> Scale.
    /// 
    /// @return 4x4 transformation matrix.
    [[nodiscard]] glm::mat4 matrix() const {
        glm::mat4 m(1.0f);
        m = glm::translate(m, position);

        m = glm::rotate(m, rotation.x, {1, 0, 0});
        m = glm::rotate(m, rotation.y, {0, 1, 0});
        m = glm::rotate(m, rotation.z, {0, 0, 1});

        m = glm::scale(m, scale);
        return m;
    }

    /// @brief Returns the local right direction vector.
    /// @return Normalized right vector.
    [[nodiscard]] glm::vec3 right() const {
        return glm::normalize(glm::vec3(matrix()[0]));
    }

    /// @brief Returns the local up direction vector.
    /// @return Normalized up vector.
    [[nodiscard]] glm::vec3 up() const {
        return glm::normalize(glm::vec3(matrix()[1]));
    }

    /// @brief Returns the local forward direction vector.
    /// 
    /// Forward is defined as -Z (OpenGL convention).
    /// 
    /// @return Normalized forward vector.
    [[nodiscard]] glm::vec3 forward() const {
        return -glm::normalize(glm::vec3(matrix()[2]));
    }

    /// @brief Resets transform to identity.
    /// 
    /// Sets position = (0,0,0), rotation = (0,0,0), scale = (1,1,1).
    void reset() {
        *this = Transform();
    }
};
