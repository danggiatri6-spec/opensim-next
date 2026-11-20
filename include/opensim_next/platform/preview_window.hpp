#pragma once

#include "opensim_next/renderer/camera_controller.hpp"
#include "opensim_next/renderer/renderer_api.hpp"

#include <span>

namespace opensim_next::platform {
class PreviewWindow final {
public:
    [[nodiscard]] int run(std::span<const renderer::DrawItem> items,
                          renderer::CameraController& camera) const;
};
} // namespace opensim_next::platform
