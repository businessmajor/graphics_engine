#include "Object.hpp"
#include "Camera.hpp"
#include "Error.hpp"

Object::Object() {}

Object::~Object() {}

// TODO: In the future it may be good to
// think about loading a 'default' texture
// if the user forgets to do this action!
void Object::LoadTexture(std::string filename) {
  // Load our actual textures
  m_textureDiffuse.LoadTexture(filename);
}

// Initialization of object as a 'quad'
//
// This could be called in the constructor or
// otherwise 'explicitly' called this
// so we create our objects at the correct time
void Object::MakeTexturedQuad(std::string filename) {
  // Setup geometry
  // We are using a new abstraction which allows us
  // to create triangles shapes on the fly
  // Position and Texture coordinate
  m_geometry.AddVertex(-1.0f, -1.0f, 0.0f, 0.0f, 0.0f);
  m_geometry.AddVertex(1.0f, -1.0f, 0.0f, 1.0f, 0.0f);
  m_geometry.AddVertex(1.0f, 1.0f, 0.0f, 1.0f, 1.0f);
  m_geometry.AddVertex(-1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

  // Make our triangles and populate our
  // indices data structure
  m_geometry.MakeTriangle(0, 1, 2);
  m_geometry.MakeTriangle(2, 3, 0);

  // This is a helper function to generate all of the geometry
  m_geometry.Gen();

  // Create a buffer and set the stride of information
  // NOTE: How we are leveraging our data structure in order to very cleanly
  //       get information into and out of our data structure.
  m_vertexBufferLayout.CreateNormalBufferLayout(
      m_geometry.GetBufferDataSize(), m_geometry.GetIndicesSize(),
      m_geometry.GetBufferDataPtr(), m_geometry.GetIndicesDataPtr());

  // Load our actual texture
  // We are using the input parameter as our texture to load
  m_textureDiffuse.LoadTexture(filename.c_str());
}

// Initialization of object as obj mesh
void Object::MakeOBJMesh(std::string filename) {
  std::cout << "Loading mesh at " << filename << "..." << std::endl;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to load OBJ file!" << std::endl;
    return;
  }

  std::string current_mtl_name;
  std::string line;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_texcoords;
  std::vector<glm::vec3> temp_normals;

  // make vertex map with string of xyzst as key
  std::map<std::string, unsigned int> vertexMap;
  unsigned int indexCount = 0;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    // vertex (v), texture coords (vt) and normals (vn)
    if (prefix == "v") {
      glm::vec3 vertex;
      iss >> vertex.x >> vertex.y >> vertex.z;
      temp_vertices.push_back(vertex);
    } else if (prefix == "vt") {
      glm::vec2 texcoord;
      iss >> texcoord.x >> texcoord.y;
      temp_texcoords.push_back(texcoord);
    } else if (prefix == "vn") {
      glm::vec3 normal;
      iss >> normal.x >> normal.y >> normal.z;
      temp_normals.push_back(normal);
    }

    // faces
    if (prefix == "f") {
      std::string vertex1, vertex2, vertex3;
      iss >> vertex1 >> vertex2 >> vertex3;
      std::vector<std::string> faces = {vertex1, vertex2, vertex3};

      for (const auto& f : faces) {
        std::istringstream vert_stream(f);
        std::string v, vt, vn;
        std::getline(vert_stream, v, '/');
        std::getline(vert_stream, vt, '/');
        std::getline(vert_stream, vn);

        int v_idx = std::stoi(v) - 1;  // OBJ indices start at 1
        int vt_idx = vt.empty() ? -1 : std::stoi(vt) - 1;
        int vn_idx = vn.empty() ? -1 : std::stoi(vn) - 1;

        glm::vec3 pos = temp_vertices[v_idx];
        glm::vec2 tex =
            vt_idx >= 0 ? temp_texcoords[vt_idx] : glm::vec2(0.0f, 0.0f);
        glm::vec3 norm =
            vn_idx >= 0 ? temp_normals[vn_idx] : glm::vec3(0.0f, 0.0f, 1.0f);

        // make a string of pos and tex for vertex map
        std::ostringstream ss;
        ss << pos.x << "," << pos.y << "," << pos.z << ",";
        ss << tex.x << "," << tex.y;
        std::string key = ss.str();

        // Check if this vertex combination already exists
        if (vertexMap.find(key) == vertexMap.end()) {
          // New vertex, add to geometry and to map
          m_geometry.AddVertex(pos.x, pos.y, pos.z, tex.x, tex.y);
          vertexMap[key] = indexCount;
          m_geometry.AddIndex(indexCount);
          indexCount++;
        } else {
          // Vertex already exists, use the existing index
          m_geometry.AddIndex(vertexMap[key]);
        }
      }
    }

    // materials
    if (prefix == "mtllib") {
      std::string mtl_filename;
      iss >> mtl_filename >> mtl_filename;  // read past "mtllib"
      std::string folder = filename.substr(0, filename.find_last_of("\\/") + 1);
      load_material_file(folder + mtl_filename);
    } else if (prefix == "usemtl") {
      iss >> current_mtl_name >> current_mtl_name;
    }
  }

  m_geometry.Gen();

  std::cout << m_geometry.GetBufferDataSize() << " unique vertices"
            << std::endl;
  std::cout << m_geometry.GetIndicesSize() << " indices" << std::endl;

  // Create a buffer and set the stride of information
  m_vertexBufferLayout.CreateNormalBufferLayout(
      m_geometry.GetBufferDataSize(), m_geometry.GetIndicesSize(),
      m_geometry.GetBufferDataPtr(), m_geometry.GetIndicesDataPtr());

  // Load our actual texture
  // We are using the input parameter as our texture to load
  m_textureDiffuse.LoadTexture("../common/textures/tile.ppm");
}

// private helper method for OBJ meshes with material files
void Object::load_material_file(const std::string& filename) {
  std::cout << "Loading material at " << filename << "..." << std::endl;
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to load material file." << std::endl;
    return;
  }

  std::string line;
  std::string mtl_name = "";

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string prefix;
    iss >> prefix;

    if (prefix == "newmtl") {
      iss >> mtl_name;
      m_material_map[mtl_name] =
          glm::vec3(0.1f, 4.f, 7.f);  // Default value, adjust as needed
    }

    else if (prefix == "Kd") {
      float r, g, b;
      iss >> r >> g >> b;
      m_material_map[mtl_name] = glm::vec3(r, g, b);
    }

    else if (prefix == "map_Kd") {
      std::string tex_filename;
      iss >> tex_filename;
      std::string folder = filename.substr(0, filename.find_last_of("\\/") + 1);
      std::string full_tex_path = folder + tex_filename;
      std::cout << "Loading texture: " << full_tex_path << std::endl;
      m_texture_map[mtl_name] = std::make_unique<Texture>();
      m_texture_map[mtl_name]->LoadTexture(full_tex_path);
      m_texture_map[mtl_name]->Bind();
    }
  }
}

// Bind everything we need in our object
// Generally this is called in update() and render()
// before we do any actual work with our object
void Object::Bind() {
  // Make sure we are updating the correct 'buffers'
  m_vertexBufferLayout.Bind();
  // Diffuse map is 0 by default, but it is good to set it explicitly
  m_textureDiffuse.Bind(0);
}

// Render our geometry
void Object::Render() {
  // Call our helper function to just bind everything
  Bind();
  // Render data
  glDrawElements(
      GL_TRIANGLES,
      m_geometry.GetIndicesSize(),  // The number of indices, not triangles.
      GL_UNSIGNED_INT,              // Make sure the data type matches
      nullptr);                     // Offset pointer to the data.
                                    // nullptr because we are currently bound
}
