# World-to-Renderer Bridge

The bridge converts a region's client-side world objects and explicit render bindings into a stable list of DrawItem records.

## Contract

- The world model owns object identity and transforms.
- A binding associates an entity ID with mesh and material handles.
- Unbound objects are omitted until their assets are ready.
- Duplicate bindings use the first valid binding.
- Output is sorted by entity ID for deterministic frame construction.
- The bridge copies transforms and does not mutate authoritative world state.

The SDL3/OpenGL preview consumes this output directly. In this first visible integration, each valid mesh handle maps to a built-in debug cube; arbitrary mesh buffers and PBR uploads are planned for the next renderer milestone.

## Current limitations

Avatar bindings, terrain, frustum culling, LOD, batching, and GPU uploads are not implemented.
