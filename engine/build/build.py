import os
import platform
import subprocess

# (1)==================== COMMON CONFIGURATION OPTIONS ======================= #
COMPILER = "g++"
SOURCE = "./src/*.cpp"
EXECUTABLE = "engine"

# (2)=================== Platform specific configuration ===================== #
ARGUMENTS = "-std=c++20"
INCLUDE_DIR = f"-I./include/ -I./../common/thirdparty/glm"
LIBRARIES = "" #"-lSDL3"

if platform.system() == "Linux":
    ARGUMENTS += " -D LINUX"  # -D is a #define sent to preprocessor
elif platform.system() == "Darwin":
    ARGUMENTS += " -D MAC"
elif platform.system() == "Windows":
    ARGUMENTS += "-D MINGW -static-libgcc -static-libstdc++ -mwindows"
    LIBRARIES += f"-lmingw32 -L ./lib"
    EXECUTABLE += ".exe"


# (3)====================== Building the Executable ========================== #
sdl_cflags_libs = subprocess.getoutput("pkg-config sdl3 --cflags --libs")
compile_string = COMPILER+" "+ARGUMENTS+" "+SOURCE + \
    " -o " + EXECUTABLE+" "+INCLUDE_DIR+" "+LIBRARIES+f" {sdl_cflags_libs}"
# Print out the compile string
print("========================================================================")
print("Compiling " + EXECUTABLE + " on " + platform.system())
print(compile_string)
print("========================================================================")
# Run our command
exit_code = os.system(compile_string)

if exit_code == 1:
    print("Build Failed")
else:
    print("Build Successful! Run with ./" + EXECUTABLE)

exit(exit_code)
