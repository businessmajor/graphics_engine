## SDL3/OpenGL Graphics Engine

<!-- ## YouTube/Dropbox/Drive Link: https://youtu.be/M20Sm7l2iGk -->

Based on my final project for Computer Graphics during grad school. Cheers :)
Requires SDL3

<img width="1277" alt="image" src="https://github.com/user-attachments/assets/cb6e2490-202d-4e76-abbd-50875f54ed94" />

## How to run this project
*1. `cd engine`* 

*2. `python3 build/build.py`*

*3. `./engine`* 

## Project Hieararchy

### ./engine Directory Organization
```
graphics_engine/      # Project root
│
├── common/           # Shared assets across engines
│   ├── cubemap/
│   ├── objects/
│   ├── textures/
│   └── thirdparty/
│
├── engine/           # Graphics engine source code
│   ├── build/        # Build script for compiling the engine
│   │   └── build.py
│   │
│   ├── include/      # Engine headers
│   │   ├── glad/     # GLAD headers
│   │   ├── KHR/      # Khronos headers for GLAD
│   │   └── *.hpp     # Engine-specific headers
│   │
│   ├── lib/          # Dynamic libraries for Windows builds
│   │   └── *.dll
│   │
│   ├── shaders/      # GLSL shader files
│   │   └── *.glsl
│   │
│   └── src/          # Engine source code
│       ├── main.cpp  # Entry point
│       └── *.cpp     # Other source files
│    
└── README.md         # Project-wide README
```
