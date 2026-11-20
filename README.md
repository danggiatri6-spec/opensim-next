# OpenSim-next PC Viewer

An open, modern PC client for OpenSim-based virtual worlds.

**Current status:** a tested C++20 foundation with a world model, renderer API, mock backend, world-to-renderer bridge, orbit camera controller, and an SDL3/OpenGL compatibility-profile graphics preview. The preview displays a rotating test cube; it is not yet the production renderer, does not yet render World Model draw items, and does not connect to an OpenSim Grid or implement Firestorm-compatible networking.

## Product direction

OpenSim-next aims to preserve the social and creative strengths of Second Life-style virtual worlds while modernizing the client architecture.

- PC-first development.
- OpenSim compatibility first; renderer modernization in stages.
- Firestorm is a protocol and feature reference, not a dependency that is blindly copied.
- Backend-independent world state and renderer interfaces.
- Legacy asset compatibility alongside a gradual glTF/GLB and PBR path.
- Low-spec graphics mode as a first-class requirement.
- AI assists human communication and creation; it does not replace human residents.

## Repository boundaries

- `danggiatri6-spec/opensim`: simulator/server development.
- `danggiatri6-spec/opensim-next`: this PC Viewer.
- The two repositories communicate through documented OpenSim protocols and compatibility tests.

## Initial build

Requirements:
- CMake 3.20+
- A C and C++20 compiler (MSVC 2022, GCC 11+, or Clang 14+)
- Network access on the first configure to fetch SDL3
- An OpenGL 2.1 compatibility-capable graphics driver for the interactive preview

Build:

```sh
cmake -S . -B build
cmake --build build --config Release
```

Run the current headless pipeline preview:

```sh
./build/opensim-next-viewer
```

On Windows multi-configuration generators, the executable may be under `build/Release/`.

## First vertical slice

The first real client milestone is:

1. Select an OpenSim grid.
2. Authenticate using the existing login flow.
3. Connect to one region.
4. Receive and display terrain and basic legacy primitives.
5. Spawn an avatar and support basic movement.
6. Send and receive local chat.
7. Disconnect cleanly with useful diagnostics.

The executable first validates a local World Model-to-MockRenderer frame submission, then opens an SDL3/OpenGL compatibility preview window. The visible rotating cube is currently a graphics bring-up test, separate from the mock-rendered World Model object.

## Architecture

See [docs/architecture/overview.md](docs/architecture/overview.md) and [docs/development/roadmap.md](docs/development/roadmap.md).

## License

License and third-party dependency notices must be finalized before importing or distributing upstream Firestorm-derived code. Do not assume that this starter repository grants rights to third-party assets or dependencies.
