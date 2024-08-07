/** @file Object.hpp
 *  @brief Sets up an OpenGL camera..
 *
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <glad/glad.h>

#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "Geometry.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Transform.hpp"
#include "VertexBufferLayout.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/vec3.hpp"

// Purpose:
// An abstraction to create multiple objects
//
//
class Object {
 public:
  // Object Constructor
  Object();
  // Object destructor
  ~Object();
  // Load a texture
  void LoadTexture(std::string fileName);
  // Create a textured quad
  void MakeTexturedQuad(std::string fileName);
  // create a mesh
  void MakeOBJMesh(std::string filename);
  // How to draw the object
  virtual void Render();

 protected:  // Classes that inherit from Object are intended to be overridden.
  // Helper method for when we are ready to draw or update our object
  void Bind();
  // For now we have one buffer per object.
  VertexBufferLayout m_vertexBufferLayout;
  // diffuse, normal, and detail maps
  Texture m_textureDiffuse;
  Texture m_normalMap;
  Texture m_detailMap;
  // Store the objects Geometry
  Geometry m_geometry;

 private:
  void load_material_file(const std::string& filename);
  std::map<std::string, glm::vec3> m_material_map;
  std::map<std::string, std::unique_ptr<Texture>> m_texture_map;
};

#endif
