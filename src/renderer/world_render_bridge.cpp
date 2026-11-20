#include "opensim_next/renderer/world_render_bridge.hpp"

#include <algorithm>
#include <unordered_map>

namespace opensim_next::renderer {

std::vector<DrawItem> WorldRenderBridge::buildDrawItems(
    const world::RegionState& region,
    std::span<const RenderBinding> bindings) {
    std::unordered_map<world::EntityId, const RenderBinding*> bindingByEntity;
    bindingByEntity.reserve(bindings.size());

    for (const auto& binding : bindings) {
        if (binding.entityId != 0 && binding.mesh && binding.material) {
            bindingByEntity.try_emplace(binding.entityId, &binding);
        }
    }

    std::vector<DrawItem> drawItems;
    drawItems.reserve(region.objects.size());

    for (const auto& [entityId, object] : region.objects) {
        const auto binding = bindingByEntity.find(entityId);
        if (binding == bindingByEntity.end()) {
            continue;
        }

        drawItems.push_back({
            entityId,
            binding->second->mesh,
            binding->second->material,
            object.transform
        });
    }

    std::sort(drawItems.begin(), drawItems.end(),
        [](const DrawItem& left, const DrawItem& right) {
            return left.entityId < right.entityId;
        });

    return drawItems;
}

} // namespace opensim_next::renderer
