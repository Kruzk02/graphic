#include "application.h"

#include "layout.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"

constexpr float ROT_SPEED = glm::radians(90.0f);
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;

glm::vec3 getForward(const Transform &t) {
    return glm::normalize(glm::vec3(
        glm::sin(t.rotation.y) * glm::cos(t.rotation.x),
        glm::sin(t.rotation.x),
        -glm::cos(t.rotation.y) * glm::cos(t.rotation.x)
    ));
}

glm::vec3 getRight(const Transform &t) {
    return glm::normalize(glm::cross(glm::vec3(0, 1, 0), getForward(t)));
}

Application::Application(const AppConfig &config)
    : config(config),
      window(config.width, config.height),
      camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
    auto *nativeWindow = window.getNativeWindow();

    glfwSetWindowUserPointer(nativeWindow, this);
    glfwSetCursorPosCallback(nativeWindow, mouseCallback);
    glfwSetScrollCallback(nativeWindow, scrollCallback);
    glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
}

void Application::run() {
    const Shader myShader(config.shaderVertex, config.shaderFragment);

    const std::vector vertices = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f
    };

    const std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12,
        16, 17, 18, 18, 19, 16,
        20, 21, 22, 22, 23, 20
    };

    const VertexLayout layout{
        {
            {0, 3, GL_FLOAT, GL_FALSE, 0},
            {1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)}
        },
        5 * sizeof(float)
    };

    const Mesh mesh{vertices, indices, layout};
    const Texture texture{"asset/wall.jpg"};

    myShader.use();
    myShader.setInt("uTexture", 0);

    Transform transform;

    while (!window.shouldClose()) {
        updateDeltaTime();
        processInput();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Move Up
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_8) == GLFW_PRESS) {
            transform.position += transform.up() * speed * deltaTime;
        }

        // Move Down
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_2) == GLFW_PRESS) {
            transform.position -= transform.up() * speed * deltaTime;
        }

        // Move Left
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_4) == GLFW_PRESS) {
            transform.position -= transform.right() * speed * deltaTime;
        }

        // Move Right
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_6) == GLFW_PRESS) {
            transform.position += transform.right() * speed * deltaTime;
        }

        // Move forward
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_7) == GLFW_PRESS) {
            transform.position += transform.forward() * speed * deltaTime;
        }

        // Move backward
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_9) == GLFW_PRESS) {
            transform.position -= transform.forward() * speed * deltaTime;
        }

        // Rotate down
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_1) == GLFW_PRESS) {
            transform.rotation.x += ROT_SPEED * deltaTime;
        }

        // Rotate up
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_3) == GLFW_PRESS) {
            transform.rotation.x -= ROT_SPEED * deltaTime;
        }

        // Rotate left
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_0) == GLFW_PRESS) {
            transform.rotation.y -= ROT_SPEED * deltaTime;
        }

        // Rotate right
        if (glfwGetKey(window.getNativeWindow(), GLFW_KEY_KP_DECIMAL) == GLFW_PRESS) {
            transform.rotation.y += ROT_SPEED * deltaTime;
        }

        transform.rotation.x = glm::clamp(
            transform.rotation.x,
            glm::radians(-89.0f),
            glm::radians(89.0f)
        );

        myShader.use();

        myShader.setMat4("uModel", transform.matrix());
        myShader.setMat4("uView", camera.getViewMatrix());
        myShader.setMat4("uProjection", glm::perspective(
                             glm::radians(camera.getZoom()),
                             900.0f / 720.0f,
                             NEAR_PLANE,
                             FAR_PLANE
                         ));

        texture.bind();
        mesh.draw();

        glfwPollEvents();
        glfwSwapBuffers(window.getNativeWindow());
    }
}

void Application::updateDeltaTime() {
    const auto now = static_cast<float>(glfwGetTime());
    deltaTime = now - lastFrame;
    lastFrame = now;
}

void Application::processInput() {
    auto *w = window.getNativeWindow();
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(w, true);

    if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::LEFT, deltaTime);
    if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}

void Application::mouseCallback(GLFWwindow *window, const double xPos, const double yPos) {
    auto *app =
            static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app) return;

    auto &[lastX, lastY, firstMouse] = app->mouseState;

    if (firstMouse) {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
        return;
    }

    const float xOffset = static_cast<float>(xPos) - lastX;
    const float yOffset = lastY - static_cast<float>(yPos);

    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);

    app->camera.processMouseMovement(xOffset, yOffset);
}

void Application::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
    if (!app) return;

    app->camera.processMouseScroll(static_cast<float>(yOffset));
}
