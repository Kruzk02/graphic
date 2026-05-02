#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Window
{
  public:
    explicit Window(int width, int height, const char *title = "window");
    ~Window();

    [[nodiscard]] auto shouldClose() const -> bool
    {
        return glfwWindowShouldClose(window);
    }

    void swapBuffers() const
    {
        glfwSwapBuffers(window);
    }

    [[nodiscard]] auto getNativeWindow() const -> GLFWwindow *
    {
        return window;
    }

  private:
    GLFWwindow *window;

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
};
