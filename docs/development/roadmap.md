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

## Milestone 2 — Real graphics backend (prototype track)
- [x] Move preview GPU cache into a handle-based OpenGL mesh resource registry
- [ ] Add shader-based OpenGL backend
- [ ] Add GPU resource lifecycle and capability reporting
- [ ] Add basic terrain and avatar placeholders
- [ ] Add frustum culling and simple LOD
- [ ] Validate low/medium/high quality profiles on integrated graphics

## Milestone F0 — Firestorm upstream integration (start now; parallel to prototype)
- [ ] Pin an official Firestorm release baseline and exact commit
- [ ] Create a dedicated Firestorm-based integration branch/fork while preserving upstream history
- [ ] Build unmodified OpenSim-enabled Firestorm on Windows 10/11
- [ ] Connect to a test OpenSim Grid and verify login, region, avatar, inventory, chat and basic building
- [ ] Establish repeatable build instructions and smoke-test automation
- [ ] Record license, dependency and patch-provenance requirements

See [Firestorm integration strategy](firestorm-integration.md), [baseline build audit](firestorm-baseline-audit.md), [source bootstrap and Windows build steps](firestorm-source-bootstrap.md), and [module reuse map](../architecture/firestorm-module-map.md).

## Milestone 3 — OpenSim-next adapters and product integration
- [ ] Add OpenSim-next product branding, onboarding and default Grid configuration
- [ ] Adapt Firestorm protocol/world events into OpenSim-next service boundaries
- [ ] Define ownership of replicated world state to avoid duplicate authoritative models
- [ ] Integrate selected OpenSim-next renderer experiments behind stable adapters
- [ ] Modernize UI, asset pipeline and rendering incrementally with regression tests

## Milestone 4 — Modern Viewer experience
- [ ] Avatar rendering and animation
- [ ] Modern inventory/build UI
- [ ] Social and communication interface
- [ ] Low/medium/high graphics profiles

## Renderer integration progress

The SDL3/OpenGL preview is an architecture prototype only. Firestorm integration is now a parallel priority: use Firestorm as the mature functional baseline and migrate OpenSim-next improvements incrementally rather than recreating the complete Viewer stack.
