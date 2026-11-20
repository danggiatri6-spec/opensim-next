# Renderer API contract

## Purpose

The renderer API separates world state and viewer behavior from graphics backends. The first implementation is a mock backend used for contract tests. It does not draw pixels and is not a production renderer.

## Boundary

- World state owns region, avatar, object, and transform data.
- The renderer receives immutable frame data and resource handles.
- The renderer does not own network protocol state, inventory, permissions, or authoritative world state.
- A graphics backend may be replaced without changing the world model or protocol adapter.

## Current interface

`IRenderer` defines:
- initialization and resize;
- mesh, texture, and material resource creation/destruction;
- frame submission;
- capability reporting;
- shutdown.

`RenderFrame` contains a camera snapshot and a non-owning span of draw items. The caller must keep the draw-item storage alive for the duration of `renderFrame`.

Resource handles are opaque integer tokens. A zero-valued handle is invalid. Backends own the underlying GPU or CPU resource and must reject unknown handles.

## Material baseline

`MaterialDescriptor` currently exposes base color, metallic, roughness, and an optional base-color texture. This is a small PBR-oriented baseline, not a complete glTF material model. Later work can add normal, occlusion, emissive, alpha, sampler, and texture-coordinate settings without coupling the public API to a graphics API.

## Frame and coordinate conventions

The current camera and transform structures reuse the renderer-independent world math types. Coordinate-system conversion, projection conventions, clip-space details, and GPU synchronization belong inside each backend. They must not leak into world state or protocol code.

## Validation

The mock renderer contract tests cover lifecycle rules, invalid dimensions, resource creation, material parameter bounds, unknown resource rejection, frame submission, capabilities, and shutdown.

## Non-goals for this milestone

- No Vulkan, OpenGL, Direct3D, or Metal implementation yet.
- No window-system or swapchain ownership.
- No shader language or GPU memory allocator exposed publicly.
- No assumptions that every backend supports skeletal animation, shadows, or advanced materials.

## Next implementation steps

1. Add a renderer selection/configuration layer to the application shell.
2. Build a small renderable test scene and validate camera conventions.
3. Prototype a real backend behind `IRenderer`; compare integration and maintenance costs before selecting the long-term API.
