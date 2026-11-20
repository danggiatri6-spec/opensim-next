# SDL3/OpenGL World Preview

The SDL3 compatibility preview consumes the same DrawItem list produced by WorldRenderBridge from the WorldState.

## Current behavior

- Opens a resizable SDL3 window with an OpenGL 2.1 compatibility context.
- Draws one built-in low-poly cube for each valid mesh/material-bound DrawItem.
- Applies each object's position, quaternion rotation, and scale.
- Uses CameraController camera state for the view transform.
- Supports left-drag orbit, right-drag pan, mouse-wheel zoom, and Escape to exit.
- Uses deterministic debug colors derived from entity IDs.

## Deliberate limitations

This is a visible integration prototype, not the production renderer:
- Mesh handles currently select a built-in debug cube; arbitrary vertex/index buffers are not implemented.
- Material handles are validated by the mock backend; PBR material data and textures are not uploaded.
- No terrain, avatar mesh, asset loading, shaders, shadows, or OpenSim network connection.
- Fixed-function OpenGL is a temporary compatibility path. A shader-based backend will replace it.
- CI validates compilation and unit tests; it does not prove GPU rendering on a user's machine.

The important milestone is that world objects, transforms, renderer bindings, and camera state now feed the visible preview instead of drawing an unrelated hard-coded spinning cube.
