#pragma once

#include "camera.h"
#include "mesh.h"
#include "model.h"
#include "scene.h"
#include "window.h"

struct AppConfig {
  int width;
  int height;
  const char *shaderVertex;
  const char *shaderFragment;
};

struct MouseState {
  float lastX = 0.0f;
  float lastY = 0.0f;
  bool firstMouse = true;
};

class Application {
public:
  explicit Application(const AppConfig &config);

  void run();

private:
  float speed = 5.0f;

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;

  AppConfig config;
  MouseState mouseState;

  Window window;
  Camera camera;

  Scene scene;

  Mesh createCubeMesh();
  Mesh createGridMesh();
  void updateDeltaTime();

  void processInput(Model &model);

  static void mouseCallback(GLFWwindow *window, double xPos, double yPos);

  static void scrollCallback(GLFWwindow *window, double xOffset,
                             double yOffset);
};
