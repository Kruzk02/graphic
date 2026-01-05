#pragma once
#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position{0.0f}; // World space, units
    glm::vec3 rotation{0.0f}; // Euler angles in radians(local space)
    glm::vec3 scale{1.0f}; // local scale

    [[nodiscard]] glm::mat4 matrix() const {
        glm::mat4 m(1.0f);
        m = glm::translate(m, position);

        m = glm::rotate(m, rotation.x, {1, 0, 0});
        m = glm::rotate(m, rotation.y, {0, 1, 0});
        m = glm::rotate(m, rotation.z, {0, 0, 1});

        m = glm::scale(m, scale);
        return m;
    }

    [[nodiscard]] glm::vec3 right() const {
        return glm::normalize(glm::vec3(matrix()[0]));
    }

    [[nodiscard]] glm::vec3 up() const {
        return glm::normalize(glm::vec3(matrix()[1]));
    }

    [[nodiscard]] glm::vec3 forward() const {
        return -glm::normalize(glm::vec3(matrix()[2]));
    }

    void reset() {
        *this = Transform();
    }
};
