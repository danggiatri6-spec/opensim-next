#include "opensim_next/renderer/mock_renderer.hpp"

namespace opensim_next::renderer {

bool MockRenderer::initialize(const RendererConfig& config) {
    if (initialized_ || config.width == 0 || config.height == 0) {
        return false;
    }
    width_ = config.width;
    height_ = config.height;
    initialized_ = true;
    return true;
}

bool MockRenderer::resize(std::uint32_t width, std::uint32_t height) {
    if (!initialized_ || width == 0 || height == 0) {
        return false;
    }
    width_ = width;
    height_ = height;
    return true;
}

std::uint32_t MockRenderer::nextHandle() noexcept {
    if (nextHandle_ == 0) {
        ++nextHandle_;
    }
    return nextHandle_++;
}

MeshHandle MockRenderer::createMesh(std::string_view debugName) {
    if (!initialized_ || debugName.empty()) {
        return {};
    }
    const auto handle = nextHandle();
    meshes_.insert(handle);
    return {handle};
}

void MockRenderer::destroyMesh(MeshHandle handle) {
    meshes_.erase(handle.value);
}

TextureHandle MockRenderer::createTexture(std::string_view debugName) {
    if (!initialized_ || debugName.empty()) {
        return {};
    }
    const auto handle = nextHandle();
    textures_.insert(handle);
    return {handle};
}

void MockRenderer::destroyTexture(TextureHandle handle) {
    textures_.erase(handle.value);
}

MaterialHandle MockRenderer::createMaterial(const MaterialDescriptor& descriptor) {
    if (!initialized_ || descriptor.roughness < 0.0F || descriptor.roughness > 1.0F
        || descriptor.metallic < 0.0F || descriptor.metallic > 1.0F) {
        return {};
    }
    if (descriptor.baseColorTexture
        && !textures_.contains(descriptor.baseColorTexture->value)) {
        return {};
    }
    const auto handle = nextHandle();
    materials_.insert(handle);
    return {handle};
}

void MockRenderer::destroyMaterial(MaterialHandle handle) {
    materials_.erase(handle.value);
}

bool MockRenderer::renderFrame(const RenderFrame& frame) {
    if (!initialized_) {
        return false;
    }
    for (const auto& item : frame.drawItems) {
        if (!meshes_.contains(item.mesh.value)
            || !materials_.contains(item.material.value)) {
            return false;
        }
    }
    ++renderedFrameCount_;
    lastDrawItemCount_ = frame.drawItems.size();
    return true;
}

RenderCapabilities MockRenderer::capabilities() const noexcept {
    if (!initialized_) {
        return {};
    }
    return {8192, true, false, false, true};
}

void MockRenderer::shutdown() noexcept {
    initialized_ = false;
    width_ = 0;
    height_ = 0;
    meshes_.clear();
    textures_.clear();
    materials_.clear();
}

bool MockRenderer::isInitialized() const noexcept {
    return initialized_;
}

std::size_t MockRenderer::renderedFrameCount() const noexcept {
    return renderedFrameCount_;
}

std::size_t MockRenderer::lastDrawItemCount() const noexcept {
    return lastDrawItemCount_;
}

} // namespace opensim_next::renderer
