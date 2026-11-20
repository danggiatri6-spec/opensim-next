# Development Roadmap

## Product platform priorities

- [x] Establish Windows 10/11 64-bit as the first full product target
- [x] Set macOS as Phase 2, Linux as Phase 3, and Android as a later mobile phase
- [x] Define initial engineering planning allocation: Windows 75%, macOS 15%, Linux 10%
- [ ] Windows-first performance and usability validation on integrated graphics and 8 GB systems
- [ ] macOS adaptation after the Windows core experience is established
- [ ] Linux product support after the macOS phase
- [ ] Android lightweight client planning after PC maturity

Ubuntu remains in CI as a portability smoke test; it is not a statement of equal Linux product priority. See [platform support policy](platform-support.md).

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
- [ ] Move temporary preview GPU cache into a renderer-owned resource registry
- [ ] Add shader-based OpenGL backend
- [ ] Add GPU resource lifecycle and capability reporting
- [ ] Add basic terrain and avatar placeholders
- [ ] Add frustum culling and simple LOD
- [ ] Validate low/medium/high quality profiles on integrated graphics

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

The SDL3/OpenGL preview uploads validated MeshData into GPU vertex and index buffers and draws indexed triangles. The current implementation is an OpenGL 2.1 compatibility path using fixed-function client arrays; the next renderer milestone is moving GPU resource ownership into a backend registry and introducing shaders.
