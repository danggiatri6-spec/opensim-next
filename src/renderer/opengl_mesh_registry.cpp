#include "opensim_next/renderer/opengl_mesh_registry.hpp"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <unordered_map>
#include <utility>

namespace opensim_next::renderer {
namespace {
constexpr GLenum kArrayBuffer = 0x8892;
constexpr GLenum kElementArrayBuffer = 0x8893;
constexpr GLenum kStaticDraw = 0x88E4;

using GenBuffersProc = void (APIENTRY *)(GLsizei, GLuint*);
using DeleteBuffersProc = void (APIENTRY *)(GLsizei, const GLuint*);
using BindBufferProc = void (APIENTRY *)(GLenum, GLuint);
using BufferDataProc = void (APIENTRY *)(GLenum, std::ptrdiff_t, const void*, GLenum);

struct BufferFunctions {
    GenBuffersProc genBuffers{};
    DeleteBuffersProc deleteBuffers{};
    BindBufferProc bindBuffer{};
    BufferDataProc bufferData{};

    [[nodiscard]] bool load() {
        genBuffers = reinterpret_cast<GenBuffersProc>(SDL_GL_GetProcAddress("glGenBuffers"));
        deleteBuffers = reinterpret_cast<DeleteBuffersProc>(SDL_GL_GetProcAddress("glDeleteBuffers"));
        bindBuffer = reinterpret_cast<BindBufferProc>(SDL_GL_GetProcAddress("glBindBuffer"));
        bufferData = reinterpret_cast<BufferDataProc>(SDL_GL_GetProcAddress("glBufferData"));
        return genBuffers && deleteBuffers && bindBuffer && bufferData;
    }
};

struct GpuMesh {
    GLuint vertexBuffer{};
    GLuint indexBuffer{};
    GLsizei indexCount{};
};
} // namespace

struct OpenGLMeshRegistry::Impl {
    BufferFunctions functions;
    std::unordered_map<std::uint32_t, GpuMesh> meshes;
    bool initialized{false};

    void release(const GpuMesh& mesh) noexcept {
        if (!initialized || !functions.deleteBuffers) return;
        if (mesh.vertexBuffer != 0) functions.deleteBuffers(1, &mesh.vertexBuffer);
        if (mesh.indexBuffer != 0) functions.deleteBuffers(1, &mesh.indexBuffer);
    }
};

OpenGLMeshRegistry::OpenGLMeshRegistry() : impl_(std::make_unique<Impl>()) {}
OpenGLMeshRegistry::~OpenGLMeshRegistry() { shutdown(); }

bool OpenGLMeshRegistry::initialize() {
    if (!impl_ || impl_->initialized) return false;
    if (!impl_->functions.load()) return false;
    impl_->initialized = true;
    return true;
}

bool OpenGLMeshRegistry::upload(MeshHandle handle, const MeshData& mesh) {
    if (!impl_ || !impl_->initialized || !handle || !mesh.isValid()) return false;
    if (impl_->meshes.contains(handle.value)) return false;

    constexpr auto maxBytes = static_cast<std::uintmax_t>(
        std::numeric_limits<std::ptrdiff_t>::max());
    const auto vertexCount = static_cast<std::uintmax_t>(mesh.vertices.size());
    const auto indexCount = static_cast<std::uintmax_t>(mesh.indices.size());
    if (vertexCount > maxBytes / sizeof(MeshVertex)
        || indexCount > maxBytes / sizeof(std::uint32_t)
        || indexCount > static_cast<std::uintmax_t>(
            std::numeric_limits<GLsizei>::max())) {
        return false;
    }
    const auto vertexBytes = vertexCount * sizeof(MeshVertex);
    const auto indexBytes = indexCount * sizeof(std::uint32_t);

    while (glGetError() != GL_NO_ERROR) {}
    GpuMesh gpu;
    impl_->functions.genBuffers(1, &gpu.vertexBuffer);
    impl_->functions.genBuffers(1, &gpu.indexBuffer);
    if (gpu.vertexBuffer == 0 || gpu.indexBuffer == 0 || glGetError() != GL_NO_ERROR) {
        impl_->release(gpu);
        return false;
    }

    impl_->functions.bindBuffer(kArrayBuffer, gpu.vertexBuffer);
    impl_->functions.bufferData(kArrayBuffer, static_cast<std::ptrdiff_t>(vertexBytes),
                                mesh.vertices.data(), kStaticDraw);
    impl_->functions.bindBuffer(kElementArrayBuffer, gpu.indexBuffer);
    impl_->functions.bufferData(kElementArrayBuffer, static_cast<std::ptrdiff_t>(indexBytes),
                                mesh.indices.data(), kStaticDraw);
    impl_->functions.bindBuffer(kArrayBuffer, 0);
    impl_->functions.bindBuffer(kElementArrayBuffer, 0);

    if (glGetError() != GL_NO_ERROR) {
        impl_->release(gpu);
        return false;
    }
    gpu.indexCount = static_cast<GLsizei>(mesh.indices.size());
    impl_->meshes.emplace(handle.value, gpu);
    return true;
}

bool OpenGLMeshRegistry::draw(MeshHandle handle) const {
    if (!impl_ || !impl_->initialized || !handle) return false;
    const auto found = impl_->meshes.find(handle.value);
    if (found == impl_->meshes.end()) return false;

    const GpuMesh& gpu = found->second;
    impl_->functions.bindBuffer(kArrayBuffer, gpu.vertexBuffer);
    impl_->functions.bindBuffer(kElementArrayBuffer, gpu.indexBuffer);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    constexpr GLsizei stride = static_cast<GLsizei>(sizeof(MeshVertex));
    glVertexPointer(3, GL_FLOAT, stride,
        reinterpret_cast<const void*>(offsetof(MeshVertex, position)));
    glNormalPointer(GL_FLOAT, stride,
        reinterpret_cast<const void*>(offsetof(MeshVertex, normal)));
    glTexCoordPointer(2, GL_FLOAT, stride,
        reinterpret_cast<const void*>(offsetof(MeshVertex, uv)));
    glDrawElements(GL_TRIANGLES, gpu.indexCount, GL_UNSIGNED_INT, nullptr);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    impl_->functions.bindBuffer(kArrayBuffer, 0);
    impl_->functions.bindBuffer(kElementArrayBuffer, 0);
    return glGetError() == GL_NO_ERROR;
}

void OpenGLMeshRegistry::destroy(MeshHandle handle) {
    if (!impl_ || !impl_->initialized || !handle) return;
    const auto found = impl_->meshes.find(handle.value);
    if (found == impl_->meshes.end()) return;
    impl_->release(found->second);
    impl_->meshes.erase(found);
}

void OpenGLMeshRegistry::clear() {
    if (!impl_) return;
    for (const auto& entry : impl_->meshes) impl_->release(entry.second);
    impl_->meshes.clear();
}

void OpenGLMeshRegistry::shutdown() noexcept {
    if (!impl_) return;
    clear();
    impl_->initialized = false;
    impl_->functions = {};
}

bool OpenGLMeshRegistry::isInitialized() const noexcept {
    return impl_ && impl_->initialized;
}

std::size_t OpenGLMeshRegistry::size() const noexcept {
    return impl_ ? impl_->meshes.size() : 0;
}
} // namespace opensim_next::renderer
