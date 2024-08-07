// Grid.hpp
#ifndef GRID_HPP
#define GRID_HPP

#include <memory>
#include <string>
#include <vector>
#include "SceneNode.hpp"

class Grid {
 public:
  // Constructor
  Grid(int width,
       int height,
       const std::string& cubeMeshPath,
       const std::string& cubeTexturePath,
       const std::string& vertShader,
       const std::string& fragShader);
  ~Grid();

  void Render();
  void Update(glm::mat4 projectionMatrix, Camera* camera);

 private:
  int width, height;
  std::vector<std::unique_ptr<SceneNode>> nodes;
};

#endif  // GRID_HPP
