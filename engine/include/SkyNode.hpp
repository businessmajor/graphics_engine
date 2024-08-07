#ifndef SKYNODE_HPP
#define SKYNODE_HPP

#include <memory>
#include "SceneNode.hpp"
#include "Skybox.hpp"

class SkyNode : public SceneNode {
 public:
  SkyNode(std::shared_ptr<Skybox> ob);
  virtual void Update(glm::mat4 projectionMatrix, Camera* camera) override;
};

#endif
