# OpenSim-next PC Viewer

OpenSim-next is an independent PC Viewer project for a modern OpenSim-compatible virtual world.

## Product and platform priorities

The first product target is **Windows 10/11 64-bit**. Development, usability, performance work, and release validation prioritize ordinary Windows PC users, including systems with integrated graphics.

- **Windows 10/11 64-bit — Phase 1:** primary consumer platform and the only platform targeted for full initial functionality and optimization.
- **macOS — Phase 2:** consumer platform after the Windows core experience is established.
- **Linux — Phase 3:** supplementary platform; maintain sensible portability without diverting early product resources.
- **Android — later:** lightweight companion/client planning begins after the PC product is mature.

Initial Viewer engineering-resource planning is Windows 75%, macOS 15%, and Linux 10%. These are planning allocations, not market-share measurements. Android is outside the initial PC Viewer allocation.

Ubuntu CI is retained as a portability smoke test. It does not mean Linux has the same product priority or release commitment as Windows. macOS CI and full support are deferred until Phase 2.

See [platform support policy](docs/development/platform-support.md).

## Current implementation

The repository contains a C++20 application foundation, a tested world-state model, a renderer abstraction, a world-to-renderer bridge, a camera controller, CPU mesh data and primitive factories, a handle-based OpenGL GPU mesh registry, and an SDL3/OpenGL compatibility preview. The preview uploads validated vertex/index geometry to GPU VBO/IBO buffers through the registry and draws indexed triangles. It optionally uses a GLSL 1.20 basic directional-light shader, with a fixed-function fallback, and supports mouse orbit, pan, and zoom. It is an integration prototype, not yet a production 3D Viewer and does not connect to an OpenSim Grid or implement Firestorm-compatible networking.

## Build

Requirements:
- Windows 10/11 64-bit is the primary development and product target
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

## Viewer development strategy

The production PC Viewer is being developed in the dedicated [opensim-next-viewer](https://github.com/danggiatri6-spec/opensim-next-viewer) repository using a **Firestorm-first** integration strategy. The Firestorm-derived client is the mature foundation for OpenSim protocol support, world synchronization, avatars, inventory, chat, and building workflows. OpenSim-next-specific product identity, Grid entry, UX, performance work, AI-assisted creation, and gradual rendering evolution will be developed on top of that foundation.

The SDL3/OpenGL code in this repository remains a technical experiment and renderer research track. It is not the production Viewer foundation and will not be expanded as a parallel reimplementation of the full client.

## Architecture

See [architecture overview](docs/architecture/overview.md), [renderer API](docs/architecture/renderer-api.md), [world-render bridge](docs/architecture/world-render-bridge.md), [camera controller](docs/architecture/camera-controller.md), and [SDL3/OpenGL preview](docs/architecture/sdl3-opengl-preview.md).

## Project status

The project is under active early-stage development. The compatibility preview currently uses an OpenGLMeshRegistry with GPU VBO/IBO buffers and an optional GLSL 1.20 basic lighting shader. It falls back to OpenGL 2.1 fixed-function rendering when GLSL is unavailable. It does not yet implement PBR materials, upload textures, load arbitrary external mesh assets, render avatars or terrain, or connect to a live Grid. The non-owning CPU geometry pointer is a temporary bridge; production CPU mesh ownership and creation will move fully behind the renderer API.
