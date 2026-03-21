#pragma once
#include "model.h"

class Scene {
public:
  void addModel(Model &&model) { models.push_back(std::move(model)); }

  void draw(const Shader &shader) const {
    for (const auto &m : models)
      m.draw(shader);
  }

private:
  std::vector<Model> models;
};
