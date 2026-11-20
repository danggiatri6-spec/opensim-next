# World-to-Renderer Bridge

The bridge converts a region's client-side world objects and explicit render bindings into a stable list of DrawItem records.

## Contract

- The world model owns object identity and transforms.
- A binding associates an entity ID with mesh and material handles, and may expose validated CPU-side MeshData.
- Unbound objects are omitted until their assets are ready.
- Bindings with invalid non-null geometry are rejected.
- Duplicate bindings use the first valid binding.
- Output is sorted by entity ID for deterministic frame construction.
- The bridge copies transforms and does not mutate authoritative world state.
- Geometry pointers are non-owning. The asset owner must keep MeshData alive while any resulting DrawItem is used.

The SDL3/OpenGL compatibility preview now consumes the supplied vertex and index arrays directly using an indexed triangle path. If a binding has no geometry pointer, the preview retains the built-in debug cube fallback. This is still a CPU/immediate-mode bring-up path; it does not yet upload vertex/index buffers to GPU memory.

## Current limitations

Avatar bindings, terrain, frustum culling, LOD, batching, GPU buffer allocation, texture sampling, and PBR shading are not implemented.
