# Raycaster engine

> [!WARNING]
> Forked from Bálint Kiss (@balintkissdev) at [this repo](https://github.com/balintkissdev/raycaster-engine)!

Basically a remake of doom that can run anywhere were a C++ 17 compiler is able to work, even web browser!

## Features

- `Up/Down` to move and `Left/Right` to turn around
- `WASD` to move and strafe
- `M` to toggle mini-map
- `N` to toggle "Night Mode" and display darkness and lighting effect
- `I` to toggle color invert
- `SPACE` to shoot

## Requirements

- C++17-standard compatible C++ compiler
- CMake 3.16 or newer

That's all. The dependencies are automatically downloaded by [Hunter](https://github.com/ruslo/hunter/).

## Build

```bash
mkdir build
cd buildR
cmake ..
cmake --build . --config Release
```

For cross-compilation, you can use one of the CMake toolchain files.

`TOOLCHAIN=cmake/toolchain/x86_64-w64-mingw32.cmake cmake ..`

### Emscripten support

This project can be built with Emscripten in order to be embedded in HTML. After you
[set up the Emscripten environment](https://emscripten.org/docs/getting_started/downloads.html),
issue these commands:

```bash
mkdir build
cd build
# Workarounds required for Emscripten and Hunter to work together on a local machine.
emcmake cmake \
  -DCMAKE_C_ABI_COMPILED=ON \
  -DCMAKE_CXX_ABI_COMPILED=ON \
  -DCMAKE_CROSSCOMPILING=ON \
  ..
emcmake cmake --build . --config Release
```

## Useful information
- Minimum map size is 15x15

## TODO List
- [ ] Move UI functions from RayCaster class to a separate UI class
- [ ] Sound Engine
- [ ] Basic map editor (selecting textures, powerups, enemies and map layout)
- [ ] Gamplay
  - [ ] Weapon selector
  - [ ] Powerups
  - [ ] Enemies
- [ ] Any fun stuff you can think of!
