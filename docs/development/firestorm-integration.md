# Firestorm Viewer integration strategy

## Decision

Begin Firestorm integration now, in parallel with the small OpenSim-next architecture prototype. Do not wait for the prototype renderer to become feature-complete. The prototype exists to establish boundaries and validate selected ideas; it is not a replacement for the mature Viewer stack.

Official upstream:
- Repository: https://github.com/FirestormViewer/phoenix-firestorm
- Upstream project describes Firestorm as an open-source Second Life-derived viewer for Second Life and OpenSim.
- Upstream README says the source is LGPL and recommends official release branches (rather than master/nightly or preview branches) when using the code for a separate viewer.
- Candidate baseline to evaluate: official release tag/branch Firestorm_7.2.5 (verify release status and exact commit before pinning).

## Integration model

Use a hybrid upstream-first strategy:

1. Keep a clean, traceable Firestorm upstream baseline in a dedicated integration branch or dedicated fork.
2. Build and run the unmodified upstream OpenSim-capable viewer on Windows 10/11 before changing it.
3. Preserve mature LLUDP/HTTP, login, region, object, avatar, inventory, asset, chat, friends, building, and viewer UI behavior wherever practical.
4. Apply OpenSim-next branding, defaults, Grid discovery/login configuration, and product-specific behavior as isolated patches.
5. Introduce OpenSim-next renderer/world abstractions behind adapters. Do not replace Firestorm's renderer merely because a new prototype exists.
6. Replace subsystems incrementally only after feature parity, performance, compatibility, and regression tests demonstrate a clear benefit.
7. Keep upstream merges/rebases possible; record every divergence and retain an upstream remote.

## What should be reused first

Reuse Firestorm's mature functionality rather than reimplementing it:
- Grid login and session lifecycle
- LLUDP message handling and HTTP capabilities
- Region/parcel and object synchronization
- Avatar appearance, rigging, animation, and attachments
- Inventory and asset pipeline
- Chat, friends, groups, presence, and notifications
- Building/editing tools and established viewer interaction patterns
- Existing rendering, texture handling, caching, and settings as the initial working baseline

Do not assume every feature is OpenSim-compatible merely because it exists in Firestorm. Validate each feature against the OpenSim build and target Grid.

## What OpenSim-next should own

- Product identity, onboarding, Grid directory and account experience
- Centralized Grid service integration and future platform services
- Modernized UI/UX and accessibility
- AI-assisted translation, creation and user-controlled tools
- New renderer APIs and selected modern rendering backend experiments
- Performance profiles for integrated graphics and 8 GB Windows systems
- Build/release automation and product telemetry/privacy policy

## Migration sequence

### F0 — Upstream baseline and build feasibility
- Pin an official Firestorm release baseline and record exact commit, tag, license files and dependency manifest.
- Create a separate integration branch/fork without overwriting the current OpenSim-next prototype.
- Build the untouched upstream OpenSim-enabled Viewer on Windows 10/11.
- Launch it against a test OpenSim Grid and record login, region entry, avatar, inventory, chat and build smoke tests.
- Establish a repeatable build environment and CI feasibility before large modifications.

### F1 — Product fork and minimal changes
- Add OpenSim-next product name, icons, channel/version metadata, default Grid settings and onboarding.
- Keep protocol and core viewer behavior unchanged.
- Add regression smoke tests and document upstream patch ownership.

### F2 — Architecture adapters
- Connect selected Firestorm world/protocol events to OpenSim-next's world model through adapters.
- Keep Firestorm networking and simulation semantics intact.
- Avoid duplicate authoritative world state; define one owner per state category.
- Expose renderer resource and frame abstractions only where they can be integrated without destabilizing the viewer.

### F3 — Incremental modernization
- Replace one subsystem at a time (UI, asset cache, renderer path, or selected tools).
- Keep a working fallback and feature flags.
- Require measurable benefit and compatibility tests before retiring a mature Firestorm subsystem.

## Repository strategy

Do not merge the entire Firestorm source tree into the current small CMake prototype using ad-hoc file copies. That would lose upstream history, create a difficult-to-maintain fork, and mix two incompatible build systems.

Preferred approach: maintain a dedicated Firestorm-based integration branch/fork with the upstream history intact. Keep the current `opensim-next` CMake prototype as a sandbox for architecture experiments until useful modules can be ported cleanly. Decide later whether the public product repository should be renamed/rebased around the Firestorm fork or whether the prototype remains a separate supporting repository.

## License and provenance

Firestorm's upstream repository identifies the viewer code as LGPL. Before distributing binaries or modified source, preserve copyright and license notices, identify modified files, satisfy applicable LGPL source/relinking obligations, and audit all bundled third-party dependencies and assets individually. This document is an engineering plan, not legal advice.

## Acceptance criteria before declaring integration successful

- Reproducible Windows 10/11 build from a pinned upstream commit.
- Viewer launches and logs into a test OpenSim Grid.
- Smoke tests cover login, region arrival, avatar appearance, inventory, chat, object interaction and basic building.
- No regression in essential Firestorm/OpenSim behavior.
- OpenSim-next branding and Grid defaults are isolated from upstream core changes.
- Upstream merge/rebase procedure is documented.
