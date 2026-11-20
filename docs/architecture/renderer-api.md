# Renderer API contract

## Purpose

The renderer API separates world state and viewer behavior from graphics backends. The first implementation is a mock backend used for contract tests. It does not draw pixels and is not a production renderer.

## Boundary

- World state owns region, avatar, object, and transform data.
- The renderer receives immutable frame data and resource handles.
- The renderer does not own network protocol state, inventory, permissions, or authoritative world state.
- A graphics backend may be replaced without changing the world model or protocol adapter.

## Current interface

`IRenderer` defines initialization and resize, mesh/texture/material resource creation and destruction, frame submission, capability reporting, and shutdown.

`RenderFrame` contains a camera snapshot and a non-owning span of draw items. The caller must keep the draw-item storage alive for the duration of frame consumption.

Each `DrawItem` has opaque mesh and material handles. It may also carry a non-owning pointer to validated CPU-side `MeshData` for the compatibility preview and backend bring-up. The geometry pointer is optional; when present, its owner must keep the MeshData alive until all consumers finish using the frame. Production backends should move ownership into a renderer resource registry and keep frame records handle-based.

## Mesh data

`MeshData` stores interleaved vertex attributes as position (3 floats), normal (3 floats), and UV (2 floats), plus a 32-bit index array. Validation rejects empty geometry, index counts not divisible by three, non-finite attributes, and out-of-range indices.

The primitive factory currently provides a unit cube (24 vertices, 36 indices) and a unit plane (4 vertices, 6 indices). Vertices are duplicated across cube faces to preserve hard face normals and independent UV seams.

## Material baseline

`MaterialDescriptor` currently exposes base color, metallic, roughness, and an optional base-color texture. This is a small PBR-oriented baseline, not a complete glTF material model. Later work can add normal, occlusion, emissive, alpha, sampler, and texture-coordinate settings without coupling the public API to a graphics API.

## Frame and coordinate conventions

The current camera and transform structures reuse the renderer-independent world math types. Coordinate-system conversion, projection conventions, clip-space details, and GPU synchronization belong inside each backend. They must not leak into world state or protocol code.

## Validation

The mock renderer contract tests cover lifecycle rules, invalid dimensions, resource creation, material parameter bounds, unknown resource rejection, frame submission, capabilities, shutdown, and optional MeshData validation. Geometry unit tests cover primitive counts and invalid indices/attributes.

## Non-goals for this milestone

- No production Vulkan, OpenGL, Direct3D, or Metal backend.
- No window-system or swapchain ownership in IRenderer.
- No shader language or GPU memory allocator exposed publicly.
- No assumptions that every backend supports skeletal animation, shadows, or advanced materials.

## Next implementation steps

1. Replace the non-owning preview geometry pointer with a renderer-owned mesh resource registry.
2. Upload vertex and index data into GPU buffers and draw through a shader-based OpenGL path.
3. Add texture upload and material binding, then compare the maintenance cost of OpenGL and Vulkan backends.
