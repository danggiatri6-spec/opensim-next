# SDL3/OpenGL World Preview

The SDL3 preview consumes the DrawItem list produced by WorldRenderBridge from WorldState and renders validated CPU mesh geometry through OpenGL.

## Current behavior

- Opens a resizable SDL3 window with an OpenGL 2.1 compatibility context.
- Uploads mesh vertices to OpenGL array buffers (VBOs) and 32-bit indices to element-array buffers (IBOs).
- Caches uploaded buffers for the lifetime of the preview window, keyed by the stable MeshData address.
- Draws indexed triangles using the mesh's position, normal, and UV attributes.
- Applies each object's position, quaternion rotation, and scale.
- Uses CameraController camera state for the view transform.
- Supports left-drag orbit, right-drag pan, mouse-wheel zoom, and Escape to exit.
- Releases cached GPU buffers before destroying the OpenGL context.

## Deliberate limitations

This is a visible integration prototype, not the production renderer:
- The VBO/IBO cache is local to the preview window and uses non-owning MeshData pointers; production ownership will move into a renderer-managed resource registry.
- It uses fixed-function client arrays and lighting in an OpenGL 2.1 compatibility context, not programmable shaders.
- Material handles are validated by the mock backend; PBR material data and textures are not uploaded.
- No terrain, avatar mesh, external asset loading, shadows, or OpenSim network connection.
- CI validates compilation and unit tests; it does not prove GPU rendering on a user's machine.

The milestone is that world objects now submit actual indexed geometry to the GPU, rather than substituting a hard-coded cube for every mesh.
