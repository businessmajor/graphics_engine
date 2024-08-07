#include "Grid.hpp"
#include <chrono>
#include <random>

Grid::Grid(int width,
           int height,
           const std::string& meshPath,
           const std::string& texturePath,
           const std::string& vertShader,
           const std::string& fragShader)
    : width(width), height(height) {
  // let's get random
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);

  std::uniform_real_distribution<float> heightDistribution(5.0f, 20.0f);
  std::uniform_real_distribution<float> widthDistribution(1.0f, 3.0f);
  std::uniform_real_distribution<float> positionOffset(0.0f, 2.0f);

  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      auto building = std::make_shared<Object>();
      building->MakeOBJMesh(meshPath);
      building->LoadTexture(texturePath);
      auto node = std::make_unique<SceneNode>(building, vertShader, fragShader);

      // Randomize building height and width
      float heightScale = heightDistribution(generator);
      float widthScale = widthDistribution(generator);
      float xOffset = positionOffset(generator);
      float yOffset = positionOffset(generator);

      // Reset the transformation
      node->GetLocalTransform().LoadIdentity();

      // Scale and translate the buildings
      node->GetLocalTransform().Scale(widthScale, heightScale, widthScale);
      node->GetLocalTransform().Translate(x * (5.0f * widthScale) + xOffset,
                                          0.0f,
                                          y * (7.0f * widthScale) + yOffset);

      nodes.push_back(std::move(node));
    }
  }
}

Grid::~Grid() {}

void Grid::Render() {
  for (auto& node : nodes) {
    node->Draw();
  }
}

void Grid::Update(glm::mat4 projectionMatrix, Camera* camera) {
  for (auto& node : nodes) {
    node->Update(projectionMatrix, camera);
  }
}
