#pragma once

#include "opensim_next/renderer/renderer_api.hpp"

#include <cstddef>
#include <cstdint>
#include <unordered_set>

namespace opensim_next::renderer {

class MockRenderer final : public IRenderer {
public:
    [[nodiscard]] bool initialize(const RendererConfig& config) override;
    [[nodiscard]] bool resize(std::uint32_t width, std::uint32_t height) override;
    [[nodiscard]] MeshHandle createMesh(std::string_view debugName) override;
    void destroyMesh(MeshHandle handle) override;
    [[nodiscard]] TextureHandle createTexture(std::string_view debugName) override;
    void destroyTexture(TextureHandle handle) override;
    [[nodiscard]] MaterialHandle createMaterial(const MaterialDescriptor& descriptor) override;
    void destroyMaterial(MaterialHandle handle) override;
    [[nodiscard]] bool renderFrame(const RenderFrame& frame) override;
    [[nodiscard]] RenderCapabilities capabilities() const noexcept override;
    void shutdown() noexcept override;

    [[nodiscard]] bool isInitialized() const noexcept;
    [[nodiscard]] std::size_t renderedFrameCount() const noexcept;
    [[nodiscard]] std::size_t lastDrawItemCount() const noexcept;

private:
    [[nodiscard]] std::uint32_t nextHandle() noexcept;

    bool initialized_{false};
    std::uint32_t width_{0};
    std::uint32_t height_{0};
    std::uint32_t nextHandle_{1};
    std::size_t renderedFrameCount_{0};
    std::size_t lastDrawItemCount_{0};
    std::unordered_set<std::uint32_t> meshes_;
    std::unordered_set<std::uint32_t> textures_;
    std::unordered_set<std::uint32_t> materials_;
};

} // namespace opensim_next::renderer
