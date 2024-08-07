#include "GridNode.hpp"

GridNode::GridNode(int width,
                   int height,
                   const std::string& meshPath,
                   const std::string& texturePath,
                   const std::string& vertShader,
                   const std::string& fragShader)
    : SceneNode(nullptr, vertShader, fragShader),
      grid(width, height, meshPath, texturePath, vertShader, fragShader) {}

void GridNode::Update(glm::mat4 projectionMatrix, Camera* camera) {
  grid.Update(projectionMatrix, camera);
}

void GridNode::Draw() {
  grid.Render();
}
