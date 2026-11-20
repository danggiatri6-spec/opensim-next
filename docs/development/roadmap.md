# Development Roadmap

## Milestone 0 — Repository and build foundation
- [x] CMake/C++20 project skeleton
- [x] Windows and Ubuntu CI
- [x] Core unit-test target

## Milestone 1 — World and renderer foundations
- [x] World Model for regions, objects, avatars, and transforms
- [x] Renderer API and mock backend
- [x] World-to-renderer bridge
- [x] Camera controller
- [x] SDL3 window and OpenGL compatibility context
- [x] Visible preview driven by World Model draw items
- [x] Camera orbit, pan, and zoom input

## Milestone 2 — Real graphics backend
- [ ] Replace debug cube handles with vertex/index mesh resources
- [ ] Add shader-based OpenGL backend
- [ ] Add GPU resource lifecycle and capability reporting
- [ ] Add basic terrain and avatar placeholders
- [ ] Add frustum culling and simple LOD

## Milestone 3 — Content and Grid integration
- [ ] glTF 2.0 asset ingestion and cache
- [ ] OpenSim login and region protocol research
- [ ] Firestorm-compatible protocol adapter design
- [ ] Live region/object state synchronization

## Milestone 4 — Modern Viewer experience
- [ ] Avatar rendering and animation
- [ ] Modern inventory/build UI
- [ ] Social and communication interface
- [ ] Low/medium/high graphics profiles


## Renderer integration progress

The SDL3/OpenGL preview now uploads validated MeshData into GPU vertex and index buffers and draws indexed triangles. The current implementation is an OpenGL 2.1 compatibility path using fixed-function client arrays; the next renderer milestone is moving GPU resource ownership into a backend registry and introducing shaders.
