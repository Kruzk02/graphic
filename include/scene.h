#pragma once
#include "model.h"
#include <vector>

class Scene {
public:
  void addModel(Model &&model) { models.push_back(std::move(model)); }

  std::vector<Model> &getModels() { return models; }

private:
  std::vector<Model> models;
};
