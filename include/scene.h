#pragma once
#include "model.h"
#include <vector>

class Scene
{
  public:
    void addModel(Model &&model)
    {
        models.push_back(std::move(model));
    }

    auto getModels() -> std::vector<Model> &
    {
        return models;
    }

  private:
    std::vector<Model> models;
};
