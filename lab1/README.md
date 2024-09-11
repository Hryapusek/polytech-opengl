# OpenGL simple animation application

## Requirements
You have to install all the following libraries and add pathes to them to PATH or LD_LIBRARY_PATH so that cmake could successfully find them
- OpenGL
- glut
- glm (header only openGL math library)

### Linux
On ubuntu just type
```sh
sudo apt install libglm-dev libgl-dev libglu-dev
```

### Windows
On windows you additionally have to install **g++**, **Ninja** build system and **cmake**. Add them all to PATH variable

## Build
### Linux
```bash
mkdir build
cd build
cmake -S .. -B .
cmake --build . --parallel 12
```

### Windows
```sh
mkdir build
cd build
cmake -S .. -B . -G Ninja
cmake --build . --parallel 12
```
