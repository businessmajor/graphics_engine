#ifndef GRIDNODE_HPP
#define GRIDNODE_HPP

#include "Grid.hpp"
#include "SceneNode.hpp"


class GridNode : public SceneNode {
 public:
  GridNode(int width,
           int height,
           const std::string& meshPath,
           const std::string& texturePath,
           const std::string& vertShader,
           const std::string& fragShader);

  void Update(glm::mat4 projectionMatrix, Camera* camera) override;
  void Draw() override;

 private:
  Grid grid;
};

#endif  // GRIDNODE_HPP
