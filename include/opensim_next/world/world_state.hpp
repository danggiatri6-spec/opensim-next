#pragma once

#include "opensim_next/world/types.hpp"

#include <string>
#include <unordered_map>

namespace opensim_next::world {

struct RegionState {
    RegionDescriptor descriptor;
    std::unordered_map<EntityId, WorldObject> objects;
    std::unordered_map<EntityId, AvatarState> avatars;
};

class WorldState final {
public:
    [[nodiscard]] bool addRegion(RegionDescriptor descriptor);
    [[nodiscard]] bool removeRegion(const std::string& regionId);

    [[nodiscard]] bool addObject(const std::string& regionId, WorldObject object);
    [[nodiscard]] bool updateObjectTransform(
        const std::string& regionId,
        EntityId objectId,
        Transform transform);
    [[nodiscard]] bool removeObject(const std::string& regionId, EntityId objectId);

    [[nodiscard]] bool addAvatar(const std::string& regionId, AvatarState avatar);
    [[nodiscard]] bool updateAvatarTransform(
        const std::string& regionId,
        EntityId avatarId,
        Transform transform);
    [[nodiscard]] bool removeAvatar(const std::string& regionId, EntityId avatarId);

    [[nodiscard]] const RegionState* findRegion(const std::string& regionId) const noexcept;
    [[nodiscard]] std::size_t regionCount() const noexcept;

private:
    std::unordered_map<std::string, RegionState> regions_;
};

} // namespace opensim_next::world
