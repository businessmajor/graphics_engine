/** @file Terrain.hpp
 *  @brief Create a terrain
 *
 *  More...
 *
 *  @author Mike
 *  @bug No known bugs.
 */
#ifndef TERRAIN_HPP
#define TERRAIN_HPP

#include "Image.hpp"
#include "Object.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "VertexBufferLayout.hpp"

#include <string>
#include <vector>

class Terrain : public Object {
 public:
  // Takes in a Terrain and a filename for the heightmap.
  Terrain(unsigned int xSegs, unsigned int zSegs, std::string fileName);
  // Destructor
  ~Terrain();
  // override the initialization routine.
  void Init();
  // Loads a heightmap based on a PPM image
  // This then sets the heights of the terrain.
  void LoadHeightMap(Image image);
  // Load textures
  void LoadTextures(std::string colormap, std::string detailmap);

 private:
  // data
  unsigned int m_xSegments;
  unsigned int m_zSegments;

  // Store the height in a multidimensional array
  int* m_heightData;

  // Textures for the terrain
  // Terrains are often 'multitextured' and have multiple textures.
  std::vector<Texture> m_textures;
};

#endif
