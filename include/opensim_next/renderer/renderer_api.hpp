#pragma once

#include "opensim_next/world/types.hpp"

#include <cstdint>
#include <optional>
#include <span>
#include <string_view>

namespace opensim_next::renderer {

struct MeshHandle {
    std::uint32_t value{0};
    [[nodiscard]] explicit operator bool() const noexcept { return value != 0; }
};

struct TextureHandle {
    std::uint32_t value{0};
    [[nodiscard]] explicit operator bool() const noexcept { return value != 0; }
};

struct MaterialHandle {
    std::uint32_t value{0};
    [[nodiscard]] explicit operator bool() const noexcept { return value != 0; }
};

struct Color4 {
    float r{1.0F};
    float g{1.0F};
    float b{1.0F};
    float a{1.0F};
};

struct MaterialDescriptor {
    Color4 baseColor{};
    float metallic{0.0F};
    float roughness{1.0F};
    std::optional<TextureHandle> baseColorTexture{};
};

struct CameraState {
    world::Vec3 position{};
    world::Vec3 forward{0.0, 1.0, 0.0};
    world::Vec3 up{0.0, 0.0, 1.0};
    float verticalFieldOfViewDegrees{60.0F};
    float nearClip{0.1F};
    float farClip{1024.0F};
};

struct DrawItem {
    world::EntityId entityId{0};
    MeshHandle mesh{};
    MaterialHandle material{};
    world::Transform transform{};
};

struct RenderFrame {
    CameraState camera{};
    std::span<const DrawItem> drawItems{};
};

struct RendererConfig {
    std::uint32_t width{1280};
    std::uint32_t height{720};
    bool verticalSync{true};
};

struct RenderCapabilities {
    std::uint32_t maxTextureSize{0};
    bool physicallyBasedMaterials{false};
    bool skeletalAnimation{false};
    bool shadows{false};
    bool debugMarkers{false};
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    [[nodiscard]] virtual bool initialize(const RendererConfig& config) = 0;
    [[nodiscard]] virtual bool resize(std::uint32_t width, std::uint32_t height) = 0;
    [[nodiscard]] virtual MeshHandle createMesh(std::string_view debugName) = 0;
    virtual void destroyMesh(MeshHandle handle) = 0;
    [[nodiscard]] virtual TextureHandle createTexture(std::string_view debugName) = 0;
    virtual void destroyTexture(TextureHandle handle) = 0;
    [[nodiscard]] virtual MaterialHandle createMaterial(const MaterialDescriptor& descriptor) = 0;
    virtual void destroyMaterial(MaterialHandle handle) = 0;
    [[nodiscard]] virtual bool renderFrame(const RenderFrame& frame) = 0;
    [[nodiscard]] virtual RenderCapabilities capabilities() const noexcept = 0;
    virtual void shutdown() noexcept = 0;
};

} // namespace opensim_next::renderer
