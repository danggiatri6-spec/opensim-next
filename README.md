# OpenSim-next PC Viewer

OpenSim-next is an independent PC Viewer project for a modern OpenSim-compatible virtual world.

The current repository contains a C++20 application foundation, a tested world-state model, a renderer abstraction, a world-to-renderer bridge, a camera controller, CPU mesh data and primitive factories, and an SDL3/OpenGL compatibility preview. The preview consumes validated vertex/index geometry and supports mouse orbit, pan, and zoom. It is an integration prototype, not yet a production 3D Viewer and does not connect to an OpenSim Grid or implement Firestorm-compatible networking.

## Build

Requirements:
- CMake 3.20+
- C and C++20 compiler
- Network access on first configure to fetch SDL3
- OpenGL 2.1 compatibility driver

```bash
cmake -S . -B build -DOPENSIM_NEXT_BUILD_TESTS=ON -DOPENSIM_NEXT_ENABLE_SDL3=ON
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Set OPENSIM_NEXT_ENABLE_SDL3=OFF to build the headless core and unit tests without the interactive graphics preview.

## Architecture

See [architecture overview](docs/architecture/overview.md), [renderer API](docs/architecture/renderer-api.md), [world-render bridge](docs/architecture/world-render-bridge.md), [camera controller](docs/architecture/camera-controller.md), and [SDL3/OpenGL preview](docs/architecture/sdl3-opengl-preview.md).

## Project status

The project is under active early-stage development. The compatibility preview currently submits indexed triangles through fixed-function OpenGL immediate mode. It does not yet upload GPU vertex/index buffers, use shaders or PBR materials, load arbitrary external mesh assets, render avatars or terrain, or connect to a live Grid. The non-owning CPU geometry pointer is a temporary bridge; production resource ownership will move into a renderer-managed mesh registry.
