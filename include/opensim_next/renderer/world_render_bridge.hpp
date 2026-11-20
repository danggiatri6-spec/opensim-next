#pragma once

#include "opensim_next/renderer/renderer_api.hpp"
#include "opensim_next/world/world_state.hpp"

#include <span>
#include <vector>

namespace opensim_next::renderer {

struct RenderBinding {
    world::EntityId entityId{0};
    MeshHandle mesh{};
    MaterialHandle material{};
};

class WorldRenderBridge final {
public:
    [[nodiscard]] static std::vector<DrawItem> buildDrawItems(
        const world::RegionState& region,
        std::span<const RenderBinding> bindings);
};

} // namespace opensim_next::renderer
