#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
    public:
        explicit Window(int width = 800, int height = 600, const char* title = "window");
        ~Window();

        [[nodiscard]] bool shouldClose() const {
            return glfwWindowShouldClose(window);
        }

        void swapBuffers() const {
            glfwSwapBuffers(window);
        }

        [[nodiscard]] GLFWwindow* getNativeWindow() const {
            return window;
        }
    private:
        GLFWwindow* window;

        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
