#pragma once

#include "opensim_next/renderer/mesh_data.hpp"
#include "opensim_next/renderer/renderer_api.hpp"

#include <memory>

namespace opensim_next::renderer {

// OpenGL-context-bound GPU mesh storage. Call shutdown before destroying its GL context.
class OpenGLMeshRegistry final {
public:
    OpenGLMeshRegistry();
    ~OpenGLMeshRegistry();

    OpenGLMeshRegistry(const OpenGLMeshRegistry&) = delete;
    OpenGLMeshRegistry& operator=(const OpenGLMeshRegistry&) = delete;
    OpenGLMeshRegistry(OpenGLMeshRegistry&&) = delete;
    OpenGLMeshRegistry& operator=(OpenGLMeshRegistry&&) = delete;

    [[nodiscard]] bool initialize();
    [[nodiscard]] bool upload(MeshHandle handle, const MeshData& mesh);
    [[nodiscard]] bool draw(MeshHandle handle) const;
    void destroy(MeshHandle handle);
    void clear();
    void shutdown() noexcept;

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace opensim_next::renderer
