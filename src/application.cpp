#include "application.h"

#include "layout.h"
#include "mesh.h"
#include "model.h"
#include "shader.h"
#include "transform.h"
#include <utility>

constexpr float ROT_SPEED = glm::radians(90.0f);
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;

Application::Application(const AppConfig &config)
    : lightingShader(config.shaderVertex, config.shaderFragment),
      gridShader("asset/shader/grid.vs", "asset/shader/grid.fs"),
      gridMesh(createGridMesh()), config(config),
      window(config.width, config.height),
      camera(glm::vec3(0.0f, 0.5f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {
  auto *nativeWindow = window.getNativeWindow();

  glfwSetWindowUserPointer(nativeWindow, this);
  glfwSetCursorPosCallback(nativeWindow, mouseCallback);
  glfwSetScrollCallback(nativeWindow, scrollCallback);
  glfwSetInputMode(nativeWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_FRAMEBUFFER_SRGB);
}

void Application::setup() {
  Model model;

  model.loadFromFile("asset/models/backpack/Backpack.obj");

  model.transform.position.y = 1.5f;

  scene.addModel(std::move(model));
}

void Application::update() {
  updateDeltaTime();
  processInput();
}

void Application::render() {
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 view = camera.getViewMatrix();
  glm::mat4 projection = glm::perspective(glm::radians(camera.getZoom()),
                                          static_cast<float>(config.width) /
                                              static_cast<float>(config.height),
                                          NEAR_PLANE, FAR_PLANE);

  gridShader.use();
  gridShader.setMat4("MVP", projection * view * glm::mat4(1.0f));
  gridMesh.draw();

  lightingShader.use();
  lightingShader.setVec3("lightColor", glm::vec3(2.0f, 2.0f, 2.0f));
  lightingShader.setVec3("lightPos", glm::vec3(2.0f, 3.0f, 2.0f));
  lightingShader.setVec3("viewPos", camera.getPosition());
  lightingShader.setMat4("projection", projection);
  lightingShader.setMat4("view", view);

  for (auto &model : scene.getModels()) {
    model.transform.rotation.x = glm::clamp(
        model.transform.rotation.x, glm::radians(-89.0f), glm::radians(89.0f));
    lightingShader.setMat4("model", model.transform.matrix());
    model.draw(lightingShader);
  }
}

void Application::run() {
  setup();

  while (!window.shouldClose()) {
    update();
    render();
    glfwPollEvents();
    glfwSwapBuffers(window.getNativeWindow());
  }
}

Mesh Application::createCubeMesh() {

  const std::vector<float> vertices = {
      // ---------- Front face ----------
      -0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0, 0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 0, 0.5f,
      0.5f, 0.5f, 0, 0, 1, 1, 1, -0.5f, 0.5f, 0.5f, 0, 0, 1, 0, 1,

      // ---------- Back face ----------
      0.5f, -0.5f, -0.5f, 0, 0, -1, 0, 0, -0.5f, -0.5f, -0.5f, 0, 0, -1, 1, 0,
      -0.5f, 0.5f, -0.5f, 0, 0, -1, 1, 1, 0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 1,

      // ---------- Left face ----------
      -0.5f, -0.5f, -0.5f, -1, 0, 0, 0, 0, -0.5f, -0.5f, 0.5f, -1, 0, 0, 1, 0,
      -0.5f, 0.5f, 0.5f, -1, 0, 0, 1, 1, -0.5f, 0.5f, -0.5f, -1, 0, 0, 0, 1,

      // ---------- Right face ----------
      0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0, 0.5f, -0.5f, -0.5f, 1, 0, 0, 1, 0, 0.5f,
      0.5f, -0.5f, 1, 0, 0, 1, 1, 0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 1,

      // ---------- Bottom face ----------
      -0.5f, -0.5f, -0.5f, 0, -1, 0, 0, 0, 0.5f, -0.5f, -0.5f, 0, -1, 0, 1, 0,
      0.5f, -0.5f, 0.5f, 0, -1, 0, 1, 1, -0.5f, -0.5f, 0.5f, 0, -1, 0, 0, 1,

      // ---------- Top face ----------
      -0.5f, 0.5f, 0.5f, 0, 1, 0, 0, 0, 0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0, 0.5f,
      0.5f, -0.5f, 0, 1, 0, 1, 1, -0.5f, 0.5f, -0.5f, 0, 1, 0, 0, 1};

  std::vector<Vertex> verticesOut;

  for (size_t i = 0; i < vertices.size(); i += 8) {
    Vertex v{};

    v.Position = glm::vec3(vertices[i + 0], vertices[i + 1], vertices[i + 2]);

    v.Normal = glm::vec3(vertices[i + 3], vertices[i + 4], vertices[i + 5]);

    v.TexCoord = glm::vec2(vertices[i + 6], vertices[i + 7]);

    // Optional / default values
    v.Tangent = glm::vec3(0.0f);
    v.BiTangent = glm::vec3(0.0f);

    for (int j = 0; j < MAX_BONE_INFLUENCE; j++) {
      v.mBoneIds[j] = -1;
      v.mWeights[j] = 0.0f;
    }

    verticesOut.push_back(v);
  }

  const std::vector<unsigned int> indices = {
      0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,  8,  9,  10, 10, 11, 8,
      12, 13, 14, 14, 15, 12, 16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

  const VertexLayout layout{
      {
          {{0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position)},
           {1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Normal)},
           {2, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, TexCoord)}},
      },
      sizeof(Vertex)};

  return {verticesOut, indices, layout};
}

Mesh Application::createGridMesh() {

  const std::vector<float> gridVertices = {-1000, 0, -1000, 1000,  0, -1000,
                                           1000,  0, 1000,  -1000, 0, 1000};

  std::vector<Vertex> gridOut;
  for (size_t i = 0; i < gridVertices.size(); i += 3) {
    Vertex v{};

    v.Position = glm::vec3(gridVertices[i + 0], gridVertices[i + 1],
                           gridVertices[i + 2]);

    // Defaults for missing attributes
    v.Normal = glm::vec3(0.0f, 1.0f, 0.0f); // up
    v.TexCoord = glm::vec2(0.0f);
    v.Tangent = glm::vec3(0.0f);
    v.BiTangent = glm::vec3(0.0f);

    for (int j = 0; j < MAX_BONE_INFLUENCE; j++) {
      v.mBoneIds[j] = -1;
      v.mWeights[j] = 0.0f;
    }

    gridOut.push_back(v);
  }

  const std::vector<unsigned int> gridIndices = {0, 1, 2, 2, 3, 0};

  const VertexLayout gridLayout(
      {{0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, Position)}}, sizeof(Vertex));

  return {gridOut, gridIndices, gridLayout};
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

void Application::mouseCallback(GLFWwindow *window, const double xPos,
                                const double yPos) {
  auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (!app)
    return;

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

void Application::scrollCallback(GLFWwindow *window, double xOffset,
                                 double yOffset) {
  auto *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
  if (!app)
    return;

  app->camera.processMouseScroll(static_cast<float>(yOffset));
}
