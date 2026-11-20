# World-to-renderer bridge

## Purpose

The bridge converts renderer-independent world objects into backend-neutral draw items. It is the first integration point between the world model and the renderer API.

## Current behavior

- The caller supplies one region snapshot and a list of render bindings.
- A render binding associates a world entity ID with an existing mesh and material handle.
- Objects without a valid binding are skipped; the bridge does not invent geometry or resources.
- Duplicate bindings use the first valid binding for an entity.
- Output is sorted by entity ID so frame submission order is stable even though region objects are stored in an unordered map.
- Object transforms are copied into draw items. The bridge does not mutate authoritative world state.

## Ownership and lifetime

The returned draw-item vector owns its data. The caller must keep that vector alive until the renderer finishes consuming the corresponding RenderFrame span. Resource handles remain owned by the renderer and must not be used after their resources are destroyed or the renderer is shut down.

## Current scope limits

This first bridge handles world objects only. Avatar rendering requires an avatar mesh/rig/animation binding model and will be added separately. The bridge does not perform frustum culling, LOD selection, batching, material sorting, or GPU upload.

## Next steps

1. Add avatar render bindings once the avatar rig model is defined.
2. Add visibility and distance-based LOD selection without changing world state.
3. Introduce a real backend behind IRenderer and test the same bridge output against it.
