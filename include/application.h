#pragma once

#include "camera.h"
#include "window.h"

struct AppConfig {
    int width;
    int height;
    const char* shaderVertex;
    const char* shaderFragment;
};

struct MouseState {
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;
};

class Application {
public:
    explicit Application(const AppConfig& config);
    void run();

private:

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    AppConfig config;
    MouseState mouseState;

    Window window;
    Camera camera;

    void updateDeltaTime();
    void processInput();

    static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

};