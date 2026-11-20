#include "opensim_next/world/world_state.hpp"

#include <utility>

namespace opensim_next::world {

bool WorldState::addRegion(RegionDescriptor descriptor) {
    if (descriptor.id.empty()) {
        return false;
    }

    RegionState state;
    state.descriptor = std::move(descriptor);
    const auto id = state.descriptor.id;
    return regions_.emplace(id, std::move(state)).second;
}

bool WorldState::removeRegion(const std::string& regionId) {
    return regions_.erase(regionId) == 1;
}

bool WorldState::addObject(const std::string& regionId, WorldObject object) {
    auto region = regions_.find(regionId);
    if (region == regions_.end() || object.id == 0) {
        return false;
    }
    return region->second.objects.emplace(object.id, std::move(object)).second;
}

bool WorldState::updateObjectTransform(
    const std::string& regionId,
    EntityId objectId,
    Transform transform) {
    auto region = regions_.find(regionId);
    if (region == regions_.end()) {
        return false;
    }
    auto object = region->second.objects.find(objectId);
    if (object == region->second.objects.end()) {
        return false;
    }
    object->second.transform = transform;
    return true;
}

bool WorldState::removeObject(const std::string& regionId, EntityId objectId) {
    auto region = regions_.find(regionId);
    return region != regions_.end() && region->second.objects.erase(objectId) == 1;
}

bool WorldState::addAvatar(const std::string& regionId, AvatarState avatar) {
    auto region = regions_.find(regionId);
    if (region == regions_.end() || avatar.id == 0) {
        return false;
    }
    return region->second.avatars.emplace(avatar.id, std::move(avatar)).second;
}

bool WorldState::updateAvatarTransform(
    const std::string& regionId,
    EntityId avatarId,
    Transform transform) {
    auto region = regions_.find(regionId);
    if (region == regions_.end()) {
        return false;
    }
    auto avatar = region->second.avatars.find(avatarId);
    if (avatar == region->second.avatars.end()) {
        return false;
    }
    avatar->second.transform = transform;
    return true;
}

bool WorldState::removeAvatar(const std::string& regionId, EntityId avatarId) {
    auto region = regions_.find(regionId);
    return region != regions_.end() && region->second.avatars.erase(avatarId) == 1;
}

const RegionState* WorldState::findRegion(const std::string& regionId) const noexcept {
    const auto region = regions_.find(regionId);
    return region == regions_.end() ? nullptr : &region->second;
}

std::size_t WorldState::regionCount() const noexcept {
    return regions_.size();
}

} // namespace opensim_next::world
