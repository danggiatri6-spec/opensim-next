# OpenSim-next PC Viewer Architecture

## Product direction

OpenSim-next is a PC-first Viewer for a centralized OpenSim-compatible Grid. It aims to preserve the social and creator ecosystem of Second Life/OpenSim while gradually modernizing the client.

The first full product target is Windows 10/11 64-bit. macOS follows after the Windows core experience is established; Linux is a later supplementary platform; Android is planned only after the PC product matures. This is a product-resource priority, not a restriction on portable architecture.

## Ordinary-user performance principles

- Prioritize integrated-GPU compatibility and a usable experience on 8 GB systems.
- Offer adjustable resolution, view distance, shadows, and anti-aliasing.
- Do not require a discrete GPU for basic world access, social interaction, or building.
- Do not depend on cloud GPU rendering.
- Keep low, medium, and high quality profiles so users can trade visual fidelity for performance.

## Module boundaries

- **app-shell** — startup, window lifecycle, settings, and UI composition.
- **platform** — OS-specific windowing, input, filesystem, and integration adapters.
- **protocol** — Grid login, LLUDP/HTTP transport, session and region synchronization.
- **world** — client-side region, avatar, object, and transform state.
- **renderer-api** — backend-neutral frame, camera, and resource contracts.
- **renderer-legacy** — compatibility path for older hardware and content.
- **renderer-modern** — shader-based modern graphics backend.
- **asset-pipeline** — asynchronous download, decode, cache, and glTF ingestion.
- **social** — chat, friends, presence, voice/text assistance, and community discovery.

The protocol layer owns network communication and updates the world model. The world model is authoritative for client-side replicated state. Renderers consume immutable frame snapshots and never own protocol, inventory, permission, or Grid authority.

## Current implementation

The repository has a C++20 core, a world-state model, a backend-neutral renderer API, a mock renderer for validation, a world-to-renderer bridge, a camera controller, and an SDL3/OpenGL compatibility preview. The preview uploads indexed mesh data through VBO/IBO buffers and uses the shared camera controller for orbit, pan, and zoom.

## Viewer foundation strategy

The standalone SDL3/OpenGL implementation is an architecture prototype, not a plan to recreate all Viewer functionality. Firestorm is the intended mature functional baseline. Its OpenSim-capable networking, world synchronization, avatars, inventory, assets, social systems, building tools and established rendering behavior should be reused first, then modernized behind adapters in small, testable steps. See [Firestorm integration strategy](../development/firestorm-integration.md).

## Migration principles

- Keep protocol and renderer independent.
- Preserve compatibility with existing OpenSim/Second Life content where practical.
- Keep low-end hardware usable through configurable rendering quality.
- Keep platform-specific code behind platform interfaces; validate Windows first, then macOS, then Linux.
- Introduce modern assets and rendering incrementally.
- Keep AI assistance focused on helping real users create and communicate; do not replace users with synthetic residents.
