# Development Roadmap

## Milestone 0 — Repository and build foundation
- [x] Establish a standalone PC Viewer repository.
- [x] Add a C++20/CMake bootstrap target.
- [x] Add a small unit-test executable and CI workflow.
- [ ] Record supported compiler versions and build artifacts.
- [ ] Add formatting and static-analysis checks.

## Milestone 1 — Protocol research
- [ ] Pin a Firestorm release branch and record its upstream commit.
- [ ] Inventory login, LLUDP, CAPS, asset transfer and region state paths.
- [ ] Record LGPL and third-party dependency obligations before source import.
- [ ] Define protocol characterization fixtures.

## Milestone 2 — First client vertical slice
- [ ] Grid selection and login.
- [ ] Connect to one OpenSim region.
- [ ] Receive terrain and basic object state.
- [ ] Display one avatar and basic movement.
- [ ] Local chat send/receive.
- [ ] Clean disconnect and structured diagnostics.

## Milestone 3 — Renderer abstraction
- [ ] Define renderer lifecycle and capability interfaces.
- [ ] Add a backend-independent test scene.
- [ ] Compare Vulkan and wgpu with a small proof of concept.
- [ ] Measure low-spec and modern hardware before choosing a backend.

## Milestone 4 — Modern content and social experience
- [ ] Legacy asset compatibility tests.
- [ ] glTF/GLB import and PBR fallback.
- [ ] Avatar skeleton/animation evolution.
- [ ] User-controlled speech-to-text and optional translation.
- [ ] AI-assisted creator tools with explicit user control.

## Scope guardrails
PC is the first complete client. Do not start a mobile 3D Viewer before the PC vertical slice works. Do not rewrite the server protocol to simplify client implementation. Do not claim feature completion based only on compiling the bootstrap shell.
