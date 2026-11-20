# Mesh Data

The renderer now has a backend-neutral CPU-side geometry representation.

## Vertex layout

`MeshVertex` currently contains:
- 3D position (float)
- normal vector (float)
- UV texture coordinate (float)

Tangents, vertex colors, skinning joints and weights are intentionally deferred until the asset pipeline requires them.

## Mesh validation

`MeshData::isValid()` rejects:
- empty vertex or index arrays
- index counts that are not a multiple of three
- non-finite position, normal or UV values
- indices outside the vertex array

The validator does not yet enforce normalized normals, manifold topology, winding consistency, or material/submesh boundaries.

## Built-in primitives

- `primitives::makeUnitCube()`: 24 vertices and 36 indices, with face-specific normals and UVs.
- `primitives::makeUnitPlane()`: 4 vertices and 6 indices, lying in the XY plane with +Z normals.

Both return CPU-side data only. They are not uploaded to the GPU and are not yet consumed by the OpenGL preview. The preview still draws its temporary debug cube.

## Next integration step

Extend the renderer resource API so a mesh handle owns validated `MeshData`, then implement an OpenGL backend that uploads vertex/index buffers and draws the supplied geometry. Keep GPU resource ownership inside the backend; the World Model must remain graphics-API independent.
