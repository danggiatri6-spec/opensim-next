# SDL3/OpenGL World Preview

The SDL3 preview consumes the DrawItem list produced by WorldRenderBridge from WorldState and renders validated mesh geometry through an OpenGL 2.1 compatibility context.

## Current behavior

- Opens a resizable SDL3 window with an OpenGL 2.1 compatibility context.
- Uses the backend-specific OpenGLMeshRegistry to own GPU mesh buffers by MeshHandle.
- Uploads mesh vertices to OpenGL array buffers (VBOs) and 32-bit indices to element-array buffers (IBOs).
- Uploads each referenced mesh once before entering the frame loop.
- Draws indexed triangles using position, normal, and UV attributes.
- Applies each object's position, quaternion rotation, and scale.
- Uses CameraController camera state for the view transform.
- Supports left-drag orbit, right-drag pan, mouse-wheel zoom, and Escape to exit.
- Releases registry-owned GPU buffers before destroying the OpenGL context.

## Resource ownership

OpenGLMeshRegistry is a backend-specific resource manager. It accepts renderer MeshHandles, validates CPU MeshData, checks buffer-size and GLsizei conversions, uploads VBO/IBO data, draws by handle, and releases individual or all GPU resources. The registry stores GPU resources rather than retaining non-owning pointers to CPU MeshData.

The current preview still receives temporary CPU geometry pointers through DrawItem to perform the initial upload. A future production OpenGLRenderer will own mesh creation and registry lifetime directly through the renderer API.

## Deliberate limitations

This is a visible integration prototype, not the production renderer:
- It uses fixed-function client arrays and lighting in an OpenGL 2.1 compatibility context, not programmable shaders.
- Material handles are validated by the mock backend; PBR material data and textures are not uploaded.
- No terrain, avatar mesh, external asset loading, shadows, or OpenSim network connection.
- CI validates compilation and unit tests; it does not prove GPU rendering on a user's machine.

The milestone is that GPU mesh lifetime is now separated from window event/render-loop code and indexed world geometry is drawn through a handle-based registry.
