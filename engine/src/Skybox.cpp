#include "Skybox.hpp"

// Constructor
Skybox::Skybox() : Object() {}

Skybox::~Skybox() {}

// Initialization of object as a 'Skybox'
void Skybox::MakeSkybox() {
  const std::vector<GLfloat> skybox_vertices{
      // positions
      -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f,
      -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,

      1.0f,  -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f,

      -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,

      -1.0f, 1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
      1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f,  -1.0f,

      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, -1.0f,
      1.0f,  -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  1.0f,  -1.0f, 1.0f};

  for (int i = 0; i < skybox_vertices.size(); i += 3) {
    float x = skybox_vertices[i];
    float y = skybox_vertices[i + 1];
    float z = skybox_vertices[i + 2];

    // lol no need for texcoords with cubemap
    m_geometry.AddVertex(x, y, z, 0.0f, 0.0f);
    m_geometry.AddIndex(i / 3);
  }

  // This is a helper function to generate all of the geometry
  m_geometry.Gen();

  // Create a buffer and set the stride of information
  m_vertexBufferLayout.CreateNormalBufferLayout(
      m_geometry.GetBufferDataSize(), m_geometry.GetIndicesSize(),
      m_geometry.GetBufferDataPtr(), m_geometry.GetIndicesDataPtr());

  std::vector<std::string> filepaths = {
      "../common/cubemap/right.ppm", "../common/cubemap/left.ppm",
      "../common/cubemap/top.ppm",   "../common/cubemap/bottom.ppm",
      "../common/cubemap/back.ppm",  "../common/cubemap/front.ppm"};
  m_textureDiffuse.LoadCubeMapTexture(filepaths);
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Skybox::Bind() {
  // Make sure we are updating the correct 'buffers'
  m_vertexBufferLayout.Bind();
  m_textureDiffuse.Bind(0);
}

void Skybox::Render() {
  glDepthMask(GL_FALSE);
  glDepthFunc(GL_LEQUAL);
  //  Call our helper function to just bind everything
  Bind();
  // Render data

  glDrawElements(
      GL_TRIANGLES,
      m_geometry.GetIndicesSize(),  // The number of indices, not triangles.
      GL_UNSIGNED_INT,              // Make sure the data type matches
      nullptr);                     // Offset pointer to the data.
                                    // nullptr because we are currently bound
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LESS);
}
