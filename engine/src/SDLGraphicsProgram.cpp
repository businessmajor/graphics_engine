#include "SDLGraphicsProgram.hpp"
#include "Camera.hpp"
#include "GridNode.hpp"
#include "SkyNode.hpp"
#include "Sphere.hpp"
#include "Terrain.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Initialization function
// Returns a true or false value based on successful completion of setup.
// Takes in dimensions of window.
SDLGraphicsProgram::SDLGraphicsProgram(int w, int h) {
  // Initialization flag
  bool success = true;
  // String to hold any errors that occur.
  std::stringstream errorStream;
  // The window we'll be rendering to
  m_window = NULL;

  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    errorStream << "SDL could not initialize! SDL Error: " << SDL_GetError()
                << "\n";
    success = false;
  } else {
    // Use OpenGL 3.3 core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    // We want to request a double buffer for smooth updating.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create window
    m_window =
        SDL_CreateWindow("OpenGL Graphics Engine", w, h, SDL_WINDOW_OPENGL);

    // Check if Window did not create.
    if (m_window == NULL) {
      errorStream << "Window could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // Create an OpenGL Graphics Context
    m_openGLContext = SDL_GL_CreateContext(m_window);
    if (m_openGLContext == NULL) {
      errorStream << "OpenGL context could not be created! SDL Error: "
                  << SDL_GetError() << "\n";
      success = false;
    }

    // vsync to run at monitor refresh rate (i have 144)
    if (SDL_GL_SetSwapInterval(1) < 0) {
      std::cerr << "Unable to enable VSync! SDL Error: " << SDL_GetError()
                << std::endl;
    }

    // Initialize GLAD Library
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
      errorStream << "Failed to initialize GLAD\n";
      success = false;
    }

    // Initialize OpenGL
    if (!InitGL()) {
      errorStream << "Unable to initialize OpenGL!\n";
      success = false;
    }
  }

  // If initialization did not work, then print out a list of errors in the
  // constructor.
  if (!success) {
    errorStream
        << "SDLGraphicsProgram::SDLGraphicsProgram - Failed to initialize!\n";
    std::string errors = errorStream.str();
    SDL_Log("%s\n", errors.c_str());
  } else {
    SDL_Log(
        "SDLGraphicsProgram::SDLGraphicsProgram - No SDL, GLAD, or OpenGL, "
        "errors detected during initialization\n\n");
  }

  // SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN); // Uncomment to enable extra
  // debug support!
  GetOpenGLVersionInfo();

  // Setup our Renderer
  m_renderer = new Renderer(w, h);
}

// Proper shutdown of SDL and destroy initialized objects
SDLGraphicsProgram::~SDLGraphicsProgram() {
  if (m_renderer != nullptr) {
    delete m_renderer;
  }

  // Destroy window
  SDL_DestroyWindow(m_window);
  // Point m_window to NULL to ensure it points to nothing.
  m_window = nullptr;
  // Quit SDL subsystems
  SDL_Quit();
}

// Initialize OpenGL
// Setup any of our shaders here.
bool SDLGraphicsProgram::InitGL() {
  // Success flag
  bool success = true;

  return success;
}

// ====================== Create the planets =============
// NOTE: I will admit it is a little lazy to have these as globals,
//       we could build on our ObjectManager structure to manage the
//       each object that is inserted and automatically create a 'SceneNode'
//       for it for example. Or otherwise, build a 'SceneTree' class that
//       manages creating nodes and setting their type.
//       (e.g. A good use of the Factory Design Pattern if you'd like to
//             invetigate further--but it is beyond the scope of this
//             assignment).
Object* grass;
SceneNode* Grass;

Grid* grid;
SceneNode* City;

Sphere* sphere;
SceneNode* Mars;

Skybox* cubemap;
SceneNode* SkyBox;
// ====================== Create the planets =============

// Loops forever!
void SDLGraphicsProgram::Loop() {
  // ================== Initialize the planets ===============
  static float rotate = 0.0f;

  // Create cubemap
  cubemap = new Skybox();
  cubemap->MakeSkybox();
  std::shared_ptr<Skybox> boxPtr(cubemap);
  SkyBox = new SkyNode(boxPtr);

  // create flat plane of grass
  grass = new Object();
  grass->MakeTexturedQuad("../common/textures/mars.ppm");
  std::shared_ptr<Object> grassPtr(grass);
  Grass =
      new SceneNode(grassPtr, "./shaders/s_vert.glsl", "./shaders/s_frag.glsl");

  // create grid of buildings
  City = new GridNode(3, 5, "../common/objects/cube.obj",
                      "../common/textures/tile.ppm", "./shaders/s_vert.glsl",
                      "./shaders/s_frag.glsl");

  // Create mars
  sphere = new Sphere();
  sphere->LoadTexture("../common/textures/sun.ppm");
  std::shared_ptr<Object> spherePtr(sphere);
  Mars = new SceneNode(spherePtr, "./shaders/s_vert.glsl",
                       "./shaders/s_frag.glsl");

  // Render our scene starting from the sun.
  m_renderer->setRoot(SkyBox);
  // Define Sun's children
  SkyBox->AddChild(Grass);
  SkyBox->AddChild(City);
  SkyBox->AddChild(Mars);

  // Set a default position for our camera
  m_renderer->GetCamera(0)->SetCameraEyePosition(0.0f, 1.0f, 0.0f);

  // Main loop flag
  // If this is quit = 'true' then the program terminates.
  bool quit = false;
  // Event handler that handles various events in SDL
  // that are related to input and output
  SDL_Event e;
  // Enable text input
  SDL_StartTextInput();

  // Set the camera speed for how fast we move.
  float cameraSpeed = 1.0f;

  // While application is running
  while (!quit) {
    // Handle events on queue
    while (SDL_PollEvent(&e)) {
      // User posts an event to quit
      // An example is hitting the "x" in the corner of the window.
      if (e.type == SDL_EVENT_QUIT) {
        quit = true;
      }
      // Handle keyboard input for the camera class
      switch (e.type) {
        // Handle keyboard presses
        case SDL_EVENT_KEY_DOWN:
          switch (e.key.keysym.sym) {
            case SDLK_LEFT:
              m_renderer->GetCamera(0)->MoveLeft(cameraSpeed);
              break;
            case SDLK_RIGHT:
              m_renderer->GetCamera(0)->MoveRight(cameraSpeed);
              break;
            case SDLK_UP:
              m_renderer->GetCamera(0)->MoveForward(cameraSpeed);
              break;
            case SDLK_DOWN:
              m_renderer->GetCamera(0)->MoveBackward(cameraSpeed);
              break;
            case SDLK_RSHIFT:
              m_renderer->GetCamera(0)->MoveUp(cameraSpeed);
              break;
            case SDLK_RCTRL:
              m_renderer->GetCamera(0)->MoveDown(cameraSpeed);
              break;
            case SDLK_ESCAPE:
              quit = true;
              break;
          }
          break;

        case SDL_EVENT_MOUSE_MOTION:
          // Handle mouse movements
          int mouseX = e.motion.x;
          int mouseY = e.motion.y;
          m_renderer->GetCamera(0)->MouseLook(mouseX, mouseY);
      }
    }  // End SDL_PollEvent loop.
       // ================== Use the planets ===============
    // Adjust the rotation speed for each planet
    // float earthRotationSpeed = 0.01f;     // Adjust as needed
    // float marsRotationSpeed = 0.008f;     // Adjust as needed
    // float jupiterRotationSpeed = 0.005f;  // Adjust as needed

    // // Increment the rotation angle for each planet
    // static float earthAngle = 0.0f;
    // static float marsAngle = 0.0f;
    // static float jupiterAngle = 0.0f;

    // earthAngle += earthRotationSpeed;
    // marsAngle += marsRotationSpeed;
    // jupiterAngle += jupiterRotationSpeed;

    // SkyBox->GetLocalTransform().LoadIdentity();

    // SkyBox->GetLocalTransform().Translate(0.0f, -5.0f, 0.0f);
    // SkyBox->GetLocalTransform().Scale(250.0f, 250.0f, 250.0f);
    // SkyBox->GetLocalTransform().Rotate(1.57, 1.f, 0.f, 0.f);

    Grass->GetLocalTransform().LoadIdentity();

    Grass->GetLocalTransform().Translate(10.0f, -5.0f, 10.f);
    Grass->GetLocalTransform().Scale(512.0f, 512.0f, 512.0f);
    Grass->GetLocalTransform().Rotate(1.57, 1.f, 0.f, 0.f);

    // Sun->GetLocalTransform().LoadIdentity();
    // Sun->GetLocalTransform().Translate(5.0f, 0.0f, 0.0f);

    // Earth->GetLocalTransform().LoadIdentity();
    // // Earth->GetLocalTransform().Scale();
    // Earth->GetLocalTransform().Translate(5.f, 0.f,
    //                                      0.f);  // Set Earth's orbital radius
    // Earth->GetLocalTransform().Rotate(earthAngle, 0.f, 1.f,
    //                                   0.f);  // Rotate Earth around the Sun

    // // Moon->GetLocalTransform().LoadIdentity();
    // // ... transform the Moon

    Mars->GetLocalTransform().LoadIdentity();

    Mars->GetLocalTransform().Translate(0.f, 70.f, -20.f);
    Mars->GetLocalTransform().Scale(10.f, 10.f, 10.f);
    // Sun->GetLocalTransform().Rotate(45, 0.f, 1.f, 0.f);

    // Update our scene through our renderer
    m_renderer->Update();
    // Render our scene using our selected renderer
    m_renderer->Render();

    // Update screen of our specified window
    SDL_GL_SwapWindow(GetSDLWindow());
  }
  // Disable text input
  SDL_StopTextInput();
}

// Get Pointer to Window
SDL_Window* SDLGraphicsProgram::GetSDLWindow() {
  return m_window;
}

// Helper Function to get OpenGL Version Information
void SDLGraphicsProgram::GetOpenGLVersionInfo() {
  SDL_Log(
      "(Note: If you have two GPU's, make sure the correct one is selected)");
  SDL_Log("Vendor: %s", (const char*)glGetString(GL_VENDOR));
  SDL_Log("Renderer: %s", (const char*)glGetString(GL_RENDERER));
  SDL_Log("Version: %s", (const char*)glGetString(GL_VERSION));
  SDL_Log("Shading language: %s",
          (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
