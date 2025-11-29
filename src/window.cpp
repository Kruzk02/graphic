#include "window.h"

#include <stdexcept>

Window::Window(const int width, const int height, const char *title) {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    if (GLFWmonitor* monitor = glfwGetPrimaryMonitor()) {
        if (const GLFWvidmode* mode = glfwGetVideoMode(monitor)) {
            const int xPos = (mode -> width - width) / 2;
            const int yPos = (mode -> height - height) / 2;
            glfwSetWindowPos(window, xPos, yPos);
        }
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

Window::~Window() {
    if (window) glfwDestroyWindow(window);
    if (glfwInit()) glfwTerminate();
}

void Window::framebuffer_size_callback(GLFWwindow* window, const int width, const int height) {
    glViewport(0, 0, width, height);
}

