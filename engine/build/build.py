import os
import platform
import subprocess

# (1)==================== COMMON CONFIGURATION OPTIONS ======================= #
COMPILER = "g++"
SOURCE = "./src/*.cpp"
EXECUTABLE = "engine"

# (2)=================== Platform specific configuration ===================== #
# For each platform we need to set the following items
# Arguments needed for our program (Add others as you see fit)
ARGUMENTS = "-std=c++20"  # Specify compiler arguments
INCLUDE_DIR = "-I ./include/ -I ./../common/thirdparty/glm"          # Which directories do we want to include.
LIBRARIES = "-lSDL3"            # What libraries do we want to include

if platform.system() == "Linux":
    ARGUMENTS += " -D LINUX"  # -D is a #define sent to preprocessor
elif platform.system() == "Darwin":
    ARGUMENTS += " -D MAC"
elif platform.system() == "Windows":
    ARGUMENTS += "-D MINGW -static-libgcc -static-libstdc++ -mwindows"
    sdl_cflags_libs = subprocess.getoutput("pkg-config sdl3 --cflags --libs")
    INCLUDE_DIR += f" {sdl_cflags_libs}"
    LIBRARIES = f"-lmingw32 -L ./lib"
    EXECUTABLE += ".exe"


# (3)====================== Building the Executable ========================== #
# Build a string of our compile commands that we run in the terminal
compileString = COMPILER+" "+ARGUMENTS+" "+SOURCE + \
    " -o " + EXECUTABLE+" "+INCLUDE_DIR+" "+LIBRARIES
# Print out the compile string
print("========================================================================")
print("Compiling " + EXECUTABLE + " on " + platform.system())
print(compileString)
print("========================================================================")
# Run our command
exit_code = os.system(compileString)
if exit_code == 1:
    print("Build Failed")
    exit(1)

print("Build Successful! Run with ./" + EXECUTABLE)
exit(0)
