# OpenSim-next PC Viewer Architecture

## Product direction

OpenSim-next is a PC-first Viewer for a centralized OpenSim-compatible Grid. It aims to preserve the social and creator ecosystem of Second Life/OpenSim while gradually modernizing the client.

## Module boundaries

- **app-shell** — startup, window lifecycle, settings, and UI composition.
- **protocol** — Grid login, LLUDP/HTTP transport, session and region synchronization.
- **world** — client-side region, avatar, object, and transform state.
- **renderer-api** — backend-neutral frame, camera, and resource contracts.
- **renderer-legacy** — compatibility path for older hardware and content.
- **renderer-modern** — shader-based modern graphics backend.
- **asset-pipeline** — asynchronous download, decode, cache, and glTF ingestion.
- **social** — chat, friends, presence, voice/text assistance, and community discovery.

The protocol layer owns network communication and updates the world model. The world model is authoritative for client-side replicated state. Renderers consume immutable frame snapshots and never own protocol, inventory, permission, or Grid authority.

## Current implementation

The repository has a C++20 core, a world-state model, a backend-neutral renderer API, a mock renderer for validation, a world-to-renderer bridge, a camera controller, and an SDL3/OpenGL compatibility preview. The preview draws bound World Model objects and uses the shared camera controller for orbit, pan, and zoom.

## Migration principles

- Keep protocol and renderer independent.
- Preserve compatibility with existing OpenSim/Second Life content where practical.
- Keep low-end hardware usable through configurable rendering quality.
- Introduce modern assets and rendering incrementally.
- Keep AI assistance focused on helping real users create and communicate; do not replace users with synthetic residents.
