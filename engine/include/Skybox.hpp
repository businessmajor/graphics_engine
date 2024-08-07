#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "Camera.hpp"
#include "CubeMapTexture.hpp"
#include "Object.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"

#include <glad/glad.h>
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>

class Skybox : public Object {
 public:
  /// Constructor
  Skybox();

  ~Skybox();

  void MakeSkybox();

  // How to draw the object
  virtual void Render() override;
  // Helper method for when we are ready to draw or update our object
  void Bind();

 private:
  VertexBufferLayout m_vertexBufferLayout;
  Geometry m_geometry;
  CubeMapTexture m_textureDiffuse;
};

#endif
